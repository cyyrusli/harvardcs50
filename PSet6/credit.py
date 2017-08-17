import math
import cs50
import sys

while True:
    try:
        print ("Credit Card Number: ", end="")
        cc = int(input())
    except ValueError:
        continue
    else:
        break

i = int(cc)
add = 0
for i in range(i > 0):
    i /= 100
    add += i % 10

multiply = int(cc/10)
product = 0

for multiply in range(multiply > 0):
    multiply /= 100
    
    if  2 * (multiply % 10) > 9:
        product += (2 * (multiply % 10)) / 10;
        product += (2 * (multiply % 10)) % 10;
    else:
        product += 2 * (multiply % 10);

sum = add + product

if sum % 10 == 0:
    
    if cc > 340000000000000 and cc <350000000000000:
        print("AMEX")
    
    elif cc > 370000000000000 and cc < 380000000000000:
        print("AMEX")
    
    elif cc > 4000000000000000 and cc < 5000000000000000:
        print("VISA")
    
    elif cc > 5000000000000000 and cc < 5600000000000000:
        print("MASTERCARD")
    
    elif cc > 4000000000000000 and cc < 500000000000000:
        print("VISA")
        
    else:
        print("INVALID")
        
else:
    print("INVALID")