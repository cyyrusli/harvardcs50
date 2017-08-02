import cs50
import math
import sys

while True:
    print('Change: ', end="")
    change = float(input())
    if(change == 0 or change <0):
        print('Input should be greater than 0')
    elif(change > 0):
        
        cent = math.floor(change * 100)
        
        quarter = int(cent / 25)
        remainder = int(cent % 25)
        
        dime = int(remainder / 10)
        remainder = int(remainder % 10)
        
        nickel = int(remainder / 5)
        remainder = int(remainder % 5)
        
        coins = int(quarter + dime + nickel + remainder)
        
        print(math.floor(coins))
        break
    else:
        print('Change: ', end="")
        change = float(input())