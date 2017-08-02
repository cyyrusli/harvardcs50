import cs50
import math

while True:
    print('Height: ', end="")
    height = int(input())
    if(height > 0 and height <= 23):
        for i in range(2, height + 1):
            space = height - i
            print(' ' * space, '#' * i)
        print('#' * (height + 1))
        break
    
    else:
        print('Height: ', end="")
        height = int(input())