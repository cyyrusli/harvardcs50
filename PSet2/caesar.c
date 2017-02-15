#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    bool input = false;
    int k;
    
    do
    {
        if(argc != 2)
        {
            printf("Usage: ./caesar k \n");
            return 1;
        }
        else
        {
            k = atoi(argv[1]); // convert string argv[] to an integer k (key)
            input = true;
        }
    } while(!input);
    
    string pt;
    
    // prompt user for input 
    do 
    {
        printf("plaintext: ");
        pt = get_string();
    } while (strlen(pt) == 0); // keep asking until there is an input
    
    printf("ciphertext: ");
    
    // iterate through the string
    for (int i = 0, n = strlen(pt); i < n; i++)
    {
        if (isalpha(pt[i]))
        {
            if (islower(pt[i]))
            {
                int ascii = pt[i];
                // to wrap around from 'z' to 'a'
                ascii = ascii - 97;
                ascii = ascii + k;
                ascii = ascii % 26;
                ascii = ascii + 97;
                printf("%c", ascii);
            }
            else 
            {
                int ascii = pt[i];
                // to wrap around from 'Z' to 'A'
                ascii = ascii - 65;
                ascii = ascii + k;
                ascii = ascii % 26;
                ascii = ascii + 65;
                printf("%c", ascii);
            }
        }
        else 
        {
            printf("%c", pt[i]);
        }
    }
        
    printf("\n");
}