#include <stdio.h>
#include <cs50.h>

int main(void) {
    printf("How long do you take to shower? (In minutes): ");
    int time = get_int();
    printf("Minutes: %i\n", time);
    printf("Bottles of water equivalent: %i\n", time * 12);
    
}