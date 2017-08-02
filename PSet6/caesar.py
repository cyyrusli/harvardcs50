import cs50
import math
import sys

if len(sys.argv) < 2:
    print('Usage: python caesar.py k')
    sys.exit()
    
while True:
    k = sys.argv[1]
    k = int(k)
    
    print('plaintext: ', end="")
    pt = input()
    
    while (len(pt) == 0):
        print('plaintext: ', end="")
        pt = input()
        
    print('ciphertext: ', end="")
    
    for char in pt:
        if(char.isalpha()):
            if(char.islower()):
                ascii = ord(char)
                ascii = ascii - 97
                ascii = ascii + k
                ascii = ascii % 26
                ascii = ascii + 97
                print(chr(ascii), end="")
                
            else:
                ascii = ord(char)
                ascii = ascii - 65
                ascii = ascii + k
                ascii = ascii % 26
                ascii = ascii + 65
                print(chr(ascii), end="")
                
        else:
            print(char, end="")
                
    print('\n', end="")
    break