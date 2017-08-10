from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    
    # pick symbol owned by user and the amount
    pf_symbols = db.execute("SELECT shares, symbol FROM portfolio WHERE id=:id", id=session["user_id"])
    
    # create temp variable to store total worth
    total_cash = 0
    
    # update symbol prices and total
    for pf_symbol in pf_symbols:
        symbol = pf_symbol["symbol"]
        shares = pf_symbol["shares"]
        stock = lookup(symbol)
        total = shares * stock["price"]
        total_cash += total
        
        db.execute("UPDATE portfolio SET price=:price, total=:total WHERE id=:id AND symbol=:symbol", \
        price=usd(stock["price"]), total=usd(total), id=session["user_id"], symbol=symbol)
    
    # update user's cash
    new_cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
    
    # update user's total worth
    total_cash += new_cash[0]["cash"]
    
    # show portfolio in homepage
    new_portfolio = db.execute("SELECT * from portfolio WHERE id=:id", id=session["user_id"])
    
    return render_template("index.html", stocks=new_portfolio, cash=usd(new_cash[0]["cash"]), total=usd(total_cash))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # check for proper symbol
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("Invalid symbol")
        
        # check for proper number of shares
        try:
            shares = int(request.form.get("shares"))
            if shares < 0:
                return apology("Shares must be a positive integer")
        except:
            return apology("Shares must be a positive integer")
            
        # select user's cash
        money = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        
        # check if cash is enough
        if not money or float(money[0]["cash"]) < stock["price"] * shares:
            return apology("Not enough cash to purchase")
            
        # update history
        db.execute("INSERT INTO histories (symbol, shares, price, id) \
        VALUES(:symbol, :shares, :price, :id)", symbol=stock["symbol"], shares=shares, \
        price=usd(stock["price"]), id=session["user_id"])
        
        # update user's cash
        db.execute("UPDATE users SET cash = cash-:purchase WHERE id=:id", \
        id=session["user_id"], purchase=stock["price"] * float(shares))
        
        # choose user's shares of that symbol
        user_shares = db.execute("SELECT shares FROM portfolio WHERE id=:id AND symbol=:symbol", \
        id=session["user_id"], symbol=stock["symbol"])
        
        # if user doesn't have that symbol's shares
        if not user_shares:
            db.execute("INSERT INTO portfolio(name, shares, price, total, symbol, id) \
            VALUES(:name, :shares, :price, :total, :symbol, :id)", name=stock["name"], \
            shares=shares, price=usd(stock["price"]), total=usd(shares * stock["price"]), \
            symbol=stock["symbol"], id=session["user_id"])
            
        # increase the share count if exists
        else:
            total_shares = user_shares[0]["shares"] + shares
            db.execute("UPDATE portfolio SET shares=:shares WHERE id=:id AND symbol=:symbol", \
            shares=total_shares, id=session["user_id"], symbol=stock["symbol"])
        
        # go back to index
    return redirect(url_for("index"))

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    histories = db.execute("SELECT * from histories WHERE id=:id", id=session["user_id"])
    
    return render_template("history.html", histories=histories)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    if request.method == "POST":
        rows = lookup(request.form.get("symbol"))
        
        if not rows:
            return apology("Invalid symbol")
            
        return render_template("quoted.html", stock=rows)
        
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    if request.method == "POST":
        # make sure username was submitted
        if not request.form.get("username"):
            return apology("Please provide a username")
        
        # make sure password was submitted
        elif not request.form.get("password"):
            return apology("Please provide a password")
            
        # make sure password and verified password is the same
        elif request.form.get("password") != request.form.get("verifypassword"):
            return apology("Passwords does not match")
            
        # insert user into users and store hash of the password
        user = db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash)", \
        username=request.form.get("username"), hash=pwd_context.encrypt(request.form.get("password")))
        
        if not user:
            return apology("User already exists")
        
        # remember which user logged in
        session["user_id"] = user
        
        # redirect user to homepage
        return redirect(url_for("index"))
        
    else:
        return render_template("register.html")
    
    return apology("TODO")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    if request.method == "GET":
        return render_template("sell.html")
    else:
        # check if symbol is correct
        stock = lookup(request.form.get("symbol"))
        
        if not stock:
            return apology("Invalid symbol")
            
        # check if number of shares is valid
        try:
            shares = int(request.form.get("shares"))
            if shares < 0:
                return apology("Number of shares must be a positive integer")
        except:
            return apology("Number of shares must be a positive integer")
            
        # select the shares user chose
        user_shares = db.execute("SELECT shares FROM portfolio WHERE id=:id AND symbol=:symbol", \
        id=session["user_id"], symbol=stock["symbol"])
        
        # check if user has enough shares to sell
        if not user_shares or int(user_shares[0]["shares"]) < shares:
            return apology("Not enough shares")
            
        # update history when sold
        db.execute("INSERT INTO histories(symbol, shares, price, id) VALUES(:symbol, :shares, :price, :id)", \
        symbol=stock["symbol"], shares=-shares, price=usd(stock["price"]), id=session["user_id"])
        
        # update user's cash
        db.execute("UPDATE users SET cash = cash+:purchase WHERE id=:id", id=session["user_id"], \
        purchase=stock["price"] * float(shares))
        
        # decrease share count
        total_shares = user_shares[0]["shares"] - shares
        
        # if share count reaches 0, remove from portfolio; if not then update share count
        if total_shares == 0:
            db.execute("DELETE FROM portfolio WHERE id=:id AND symbol=:symbol", id=session["user_id"], \
            symbol=stock["symbol"])
            
        else:
            db.execute("UPDATE portfolio SET shares=:shares WHERE id=:id AND symbol=:symbol", shares=total_shares, \
            id=session["user_id"], symbol=stock["symbol"])
            
    return redirect(url_for("index"))
    
@app.route("/loan", methods=["GET", "POST"])
@login_required
def loan():
    """Get a loan."""
    
    if request.method == "POST":
        
        # make sure there are integers
        try:
            loan = int(request.form.get("loan"))
            if loan < 0:
                return apology("Loan must be a positive amount")
            elif loan > 10000:
                return apology("Unable to loan more than $10000")
                
        except:
            return apology("Loan must be a positive integer")
            
        # update user's cash
        db.execute("UPDATE users SET cash = cash+:loan WHERE id=:id", loan=loan, id=session["user_id"])
        
        return apology("Loan is successful")
        
    else:
        return render_template("loan.html")
