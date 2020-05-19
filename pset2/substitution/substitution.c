// Encoding a message


#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

void cipher(string text, string key);
// int valid(int inputs, string arg_array[]);

// Command line argument with the key should be provided

int main(int argc, string argv[])
{
    
    // Cannot pass the argument via a function -> hence integrate the whole function in main, sorry visibility
    // valid(argc, argv[]);
    
    
    // Whole fuction START (skip)
   
    if(argc == 1 || argc > 2)
    {
        printf("Usage: ./substitution key");
        return 1;
    }
    else if(strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.");
        return 1;
    }
    
    // For Loop to check for double characters; ASCII out-of range characters; 
    
    // Determine duplicates via total sum of 1-26
    
    int sum = 0;
    
    for(int i=0; i<strlen(argv[1]); i++)
    {
        if(65 <= argv[1][i] && argv[1][i] <= 90)
    {
        int index = argv[1][i] - 65;
        sum = sum + index;
    }
        else if(97 <= argv[1][i] && argv[1][i] <= 122)
    {
        int index = argv[1][i] - 97;
        sum = sum + index;
    }
        else
    {
        printf("Invalid Key.");
        return 1;
        break; 
    }
    }
    
    // Does the sum equal the expected sum? -> Duplicates
    // - 26 since the first index is counted as 0!
    
    if(sum != (26*13+13)-26)
    {
        printf("Invalid Key.");
        return 1;
    }
    
    
    // Whole function END
    
    
    string message = get_string("plaintext: ");
    printf("ciphertext: ");
    cipher(message, argv[1]);
}


// Function to decode the text with the array as the key

void cipher(string text, string key)
{
    
    // Get the the difference of ASCII A and the Char "Index of the letter in the alphabet"
    // Plug this index into the array and print the letter from the array
    
    // Iterate through the text to get indices:
    
    for (int i = 0; i < strlen(text); i++)
    {
      
      
        
        
    // If it's a lower case we want to get the lower case, if upper the upper one!
    // We don't want to change numbers!
    
    if (65 <= text[i] && text[i] <= 90)
    {
        int index = text[i] - 65;
        // We need to transform keys from the key to uppercase if they're lower case, hence toupper()
        printf("%c", toupper(key[index]));
    }
    else if(97 <= text[i] && text[i] <= 122)
    {
        int index = text[i] - 97;
        printf("%c", tolower(key[index]));
    }
    else
    {
        printf("%c", text[i]);
    }
    
    }
    
    printf("\n");
    
}





// Function to identify invalid inputs acc. to given task

int valid(int inputs, string arg_array[])
{
    if(inputs == 1 || inputs > 2)
    {
        printf("Usage: ./substitution key");
        return 1;
    }
    else if(strlen(arg_array[1]) != 26)
    {
        printf("Key must contain 26 characters.");
        return 1;
    }
    
    // For Loop to check for double characters; ASCII out-of range characters; 
    
    // Determine duplicates via total sum of 1-26
    
    int sum = 0;
    
    for(int i=0; i<strlen(arg_array[1]); i++)
    {
        if(65 <= arg_array[1][i] && arg_array[1][i] <= 90)
    {
        int index = arg_array[1][i] - 65;
        sum = sum + index;
    }
        else if(97 <= arg_array[1][i] && arg_array[1][i] <= 122)
    {
        int index = arg_array[1][i] - 97;
        sum = sum + index;
    }
        else
    {
        printf("Invalid Key.");
        return 1;
        break; 
    }
    }
    
    // Does the sum equal the expected sum? -> Duplicates
    
    if (sum != 26 * 13 + 13)
    {
        printf("Invalid Key.");
        return 1;
    }
    else
    {
        return 0;
    }
    
}  


 
    
    
    
    