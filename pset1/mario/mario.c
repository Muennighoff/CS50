#include <stdio.h>
#include <cs50.h>


int main(void)
{
    int input;
    do
    {
        input = get_int("Numver from 1-8: ");
    }
    while(input<1 || input>8);

    for(int i=1;i<input+1;i++)
    {

        for(int j=0;j<input-i;j++)
        {
            printf(" ");
        }
        for(int g=0;g<i;g++)
        {
            printf("#");
        }
        printf("  ");
        for(int h=0;h<i;h++)
        {
            printf("#");
        }
        printf("\n");
    }

}




