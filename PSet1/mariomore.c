#include <cs50.h>
#include <stdio.h>

int main(void){
    
    int height;
    int space;
    int sharp;
    
    do {
        printf("Height: ");
        height = get_int();
    }
    while (height < 0 || height >= 24);
    
    for (int i = 1; i < height + 1; i++){
        for (space = (height - (i + 1)); space >= 0; space--){
            printf(" ");
        }
        for (sharp = 2; sharp <= (i + 1); sharp++){
            printf("#");
        }
        printf("  ");
        for (sharp = 2; sharp <= (i + 1); sharp++){
            printf("#");
        }
        printf("\n");
    }
    return 0;
}