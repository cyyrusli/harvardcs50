import cs50
while True:
    try:
        print ("Height: ", end="")
        height = int(input())
    except ValueError:
        if height < 0 or height > 23:
            continue
    else:
        break

if height > 0 and height <= 23:
    for i in range (1, height):
        space = height - i
        print (' ' * (space-1), '#' * i, '', '#' * i)
    print('#' * height, '', '#' * height)