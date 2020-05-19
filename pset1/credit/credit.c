#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long cc_number = get_long("CC Number, please: ");
    //bool valid =
    int doubled_second = 0;
    int sum_first = 0;
    long divider = 10;
    int digit = 0;
    long result1 = 0;
    long result2 = 0;
    int clean1 = 0;
    int clean2 = 0;

    while(result1 != cc_number || result2 != cc_number)
    {
        if(digit%2 == 0)
        {
            // For regular numbers (not odd)
            result1 = cc_number%divider; //Getting the rest
            //printf("Result1: %i", result1);
            if(result1 == cc_number)
            {break;}
            //Cleaning
            clean1 = (result1-result2)/(divider/10);
            //printf("Clean1: %i", clean1);
            sum_first = sum_first + clean1;
            //printf("Sum_First: %i", sum_first);

        }
        else
        {
            // For odd numbers
            result2 = cc_number%divider; //Getting the rest
            //printf("Result2: %i", result2);
            //Cleaning
            // Breaking out early to not further change clean1 & the digit
            if(result1 == cc_number)
            {break;}

            clean2 = (result2-result1)/(divider/10);
            //printf("Digit: %i", clean2);
            //Ensuring that we add the digits, when it's a two digit number
            if(clean2>4)
            {
                doubled_second = doubled_second + 1 + (clean2*2)%10;
            }
            else
            {
                doubled_second = doubled_second + 2*clean2;
            }
            //printf("Doubled_Second: %i", doubled_second);
        }
        divider *= 10;
        digit++;
    }
    if((sum_first + doubled_second)%10 != 0)
    {
        printf("INVALID\n");
    }
    //Check for Mastercard:
    else if(digit==16 && clean2==5 && clean1 <= 6 && clean1 != 0)
    {
        printf("MASTERCARD\n");
    }
    else if(digit==15 && clean1==3 && (clean2==4 || clean2==7))
    {
        printf("AMEX\n");
    }
    else
    {
        printf("VISA\n");
    }

    //printf("Digit %i\n", digit);
    //printf("Clean1 %i\n", clean1);
    //printf("Clean2 %i\n", clean2);
    //printf("Divider %i\n", divider);
    //printf("Sum First %i\n", sum_first);
    //printf("Doubled Second %i\n", doubled_second);


    //while(result1 != cc_number)
    //if(digit % 2== 0)
    //result1 = cc_number % divider
    //else if (digit % == 1)
    //digit run in the while loop
    //digit++;
    //divider *= 10;


}
