#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void){
    float input = 0;
    int cent = 0;
    int quarter = 0;
    int dime = 0;
    int nickel = 0;
    int remainder = 0;
    int coins = 0;
    
    do 
    {
        printf("How much change is owed?\n ");
        input = get_float();
        if(input == 0 || input <= 0){
            printf("Input should be greater than 0\n");
        }
    }
    while(input <= 0);
    
    cent = (int)round(input*100);

    quarter = cent / 25;
    remainder = cent % 25;
    
    dime = remainder / 10;
    remainder = remainder % 10;
    
    nickel = remainder / 5;
    remainder = remainder % 5;
    
    coins = quarter + dime + nickel + remainder;
    
    printf("Number of coins: %d\n", coins);

}