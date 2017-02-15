#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

string vigenere(string pt, string key);

int main (int argc, string argv[])
{
    bool input = false;
    string key = argv[1];
    do
    {
        if(argc != 2)
        {
            printf("Usage: ./vigenere k \n");
            return 1;
        }
        else
        {
            input = true;
            // iterate over strings in argv
            for (int i = 1; i < argc; i++)
            {
                // iterate over characters in current string
                for (int j = 0, n = strlen(argv[i]); j < n; j++)
                {
                    if (!isalpha(argv[i][j])){
                        printf("Usage: ./vigenere k \n");
                        return 1;
                    }
                }
            }
        }
    } while (!input);
    
    string pt;
    
    do 
    {
        // ask for user input
        printf("plaintext: ");
        pt = get_string();
    } while (strlen(pt) == 0); // keep asking until there is an input
    
    // call the vigenere function
    string ciphertext = vigenere(pt, key);
    
    // output ciphertext
    printf("ciphertext: %s\n", ciphertext);
}

string vigenere(string pt, string key)
{
    // store length of key input
    int length = strlen(key);
    char* ct = pt;
    
    // iterate over characters in the string
    for (int i = 0, j = 0, n = strlen(pt); i < n; i++)
    {
        if (isalpha(pt[i]))
        {
            // get ascii value for j and to wrap around the keyword
            char keyvalue = (isupper(key[j % length])) ? 'A' : 'a';
            
            // get ascii value of 'A' or 'a' depending on the case
            char value = (isupper(pt[i])) ? 'A' : 'a';
            
            // get cipher value using vigenere formula
            ct[i] = ((pt[i] - value + (key[(j % length)] - keyvalue)) % 26) + value;
            j++;
        }
        else
        {
            ct[i] = pt[i];
        }
    }
    return ct;
}