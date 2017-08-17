#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void){
    int counter = 0;
    long checkAmex;
    long checkVisa;
    long checkMastercard;
    long i;
    long multiply;
    int add;
    int product;
    int sum;
    
    printf("Number: ");
    long cc = get_long_long();
    
    checkAmex = cc;
    checkVisa = cc;
    checkMastercard = cc;
    
    // check if it is an authentic credit card number
    while ( cc < 0 );       

    // sum every other digit
    for ( i = cc, add = 0; i > 0; i /= 100 )
        add += i % 10;

    // double every other digit and add those products' digits together
    for ( multiply = cc / 10, product = 0; multiply > 0; multiply /= 100 )
    {
        if ( 2 * (multiply % 10) > 9 )
        {
            product += (2 * (multiply % 10)) / 10;
            product += (2 * (multiply % 10)) % 10;
        }
        else
            product += 2 * (multiply % 10);
    }
    
    sum = add + product;
    
    if ( sum % 10 == 0 ){
    	
    	while(cc>0){
    		cc = cc/10;
    		counter += 1;
    		
    	}
    	if (counter == 15){
    		
		    // Check if it really is AMEX
		    checkAmex = checkAmex/10000000000000;
		    
		    if (checkAmex == 34 || checkAmex == 37){
		        printf("AMEX\n");
		    	
		    }
		    else {
		    	printf("Not AMEX\n");
		    }
    	}
    	else if (counter == 13){
    		
		    // Check if it really is Visa
		    checkVisa = checkVisa/1000000000000;
		    
		    if (checkVisa == 4){
		    	printf("VISA\n");
		    }
		    else {
		    	printf("Not a Visa\n");
		    }
    	}
    	else if (counter == 16){
    		
		    // Check if it is MasterCard or Visa
		    checkVisa = checkVisa/1000000000000000;
		    checkMastercard = checkMastercard/100000000000000;
		    
		    if (checkMastercard > 50 && checkMastercard <= 55){
		    	printf("MASTERCARD\n");
		    }
		    else if (checkVisa == 4) {
		    	printf("VISA\n");
		    }
		    else {
		    	printf("Not a MasterCard or Visa\n");
		    }
    	}
    	else {
	    printf("INVALID\n");
    	}
    }
    else {
    	printf("INVALID\n");
    }
}
