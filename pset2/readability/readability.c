// Getting readability score for text
// Characteristics: Longer words; Longer sentences

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
void readability_score(string text);

int main(int argc, string argv[])
{
    // Getting text input
    string text = get_string("Text: ");
    //printf("%lu char(s)\n", strlen(text));
    //printf("%i letter(s)\n", count_letters(text));
    //printf("%i word(s)\n", count_words(text));
    //printf("%i sentence(s)\n", count_sentences(text));
    readability_score(text);
    

}


int count_letters(string text)
{
    // Loop through whole text, increment only if ASCII range
    int n = strlen(text);
    int count = 0;
    
    for (int i = 0; i < n; i++)
    {
        if ((65 <= text[i] && text[i] <= 90) || (97 <= text[i] && text[i] <= 122))
        {
            count++;
        }
    }
    return count;
}


int count_words(string text)
{
    // Loop through whole text, increment for each whitespace
    int n = strlen(text);
    int count = 1;
    
    for (int i = 0; i < n; i++)
    {
        if (text[i] == 32)
        {
            count++;
        }
    }
    return count;
}


int count_sentences(string text)
{
    // Loop through whole text, increment for each whitespace
    int n = strlen(text);
    int count = 0;
    
    for (int i = 0; i < n; i++)
    {
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            count++;
        }
    }
    return count;
}

void readability_score(string text)
{
    // string in order to return Grade ...
    // Formula: index = 0.0588 * L - 0.296 * S - 15.8
    // L: avg no of letters per 100 words -> letters / words
    // S: avg no of sentences per 100 words -> sentenes / words
    
    // L calculation
    
    float l = ((float) count_letters(text) / (float) count_words(text)) * 100;
    
    
    // S calculation
    
    float s = ((float) count_sentences(text) / (float) count_words(text)) * 100;
    
    
    // Index calculation
    
    int i = round(0.0588 * l - 0.296 * s - 15.8);
    
    if (i >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (i < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", i);
    }
    
}