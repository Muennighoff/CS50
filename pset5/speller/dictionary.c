// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
#include "dictionary.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Counter for words in dict
int word_counter = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO

    // Make a copy of word:

    char word_copy[LENGTH+1];
    strcpy (word_copy, word);

    // Turn into lower case; whole word since parts might be written big

    for (int i = 0; i < strlen(word_copy); i++)
    {
        word_copy[i] = tolower(word_copy[i]);
    }

    // Get the Hashcode:
    unsigned int hash_code = hash(word_copy);

    // Check if Index itself is a hit
        
    if (strcmp(table[hash_code]->word, word_copy) == 0)
    {
        return true;
    }
    else
    {
        node *tmp = table[hash_code]->next;
        while(tmp != NULL)
        {
            if (strcmp(tmp->word, word_copy) == 0)
            {
                return true;
            }
            else
            {
                tmp = tmp->next;
            }
        }
    }
    return false;

}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    int first_letter = 0;

    // Make a copy of word
    // No need for DMA -- I won't resize it later & I already know the size.

    char word_copy[strlen(word)+1];
    strcpy(word_copy, word);
    

    first_letter = toupper(word_copy[0]);

    // A is 1, B is 2 ...
    unsigned int hash_code = first_letter - 65;
    return hash_code;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // We're handed the name of the dictionary! -> We need to extrapolate all the values first via FILE open...

    FILE *fp = fopen(dictionary, "r");
    
    // I need malloc since I don't yet know how big the word is gonna be! 
    char *dict_word = malloc(sizeof(char)*(LENGTH+2));

    for (int i = 0; i < N; i++)
    {
        // Allocate a chunk of memory; Not sure why this is necessary although we have an array and mem should already be there
        table[i] = malloc(sizeof(node));
        if (table[i] == NULL)
            {
                printf("No memory available.");
                return false;
            }
        table[i]->next = NULL;
        table[i]->word[0] = ' ';
    }

    // Create words from dicct via gets
    
    while(fgets(dict_word, LENGTH+2, fp))
    {
        int len = strlen(dict_word);
        if (len > 0 && dict_word[len-1] == '\n') dict_word[len-1] = '\0';

        int hash_code = hash(dict_word);

        // If nothing in the index yet:
        if (table[hash_code]->word[0] == ' ')
        {
            // Populate it with the word: // For Loop necessary since else we just assign the pointer to our string
            for (int j = 0; j < strlen(dict_word) + 1; j++)
            {
                table[hash_code]->word[j] = dict_word[j];
            }
        }
        else
        {
            // Create the word & malloc & put the word into a temporary n:
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                printf("No memory available.");
                return false;
            }
            for (int j = 0; j < strlen(dict_word); j++)
            {
                n->word[j] = dict_word[j];
            }
            n->next = NULL;
            // Find the place to put our pointer
            if (table[hash_code]->next != NULL)
            {
                node *tmp = table[hash_code]->next;
                // Loop through next until free one!
                while (tmp->next != NULL)
                {
                    // Or just tmp = tmp->next ?
                    node *tmp2 = tmp->next;
                    tmp = tmp2;
                }
                // Once free spot found:
                tmp->next = n;
            }
            else
            {
                table[hash_code]->next = n;
            }
            free(n);

        }
        word_counter++;
    }
    free(dict_word);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO

    // Iterate through each Array spot;
    // Either delete whats behind you or go to the end and delete while going to start
    // I only need to free the linked lists ergo if there is nothing linked; Still need to free though since assigned in load

    for (int i = 0; i < N; i++)
    {
        while (table[i]->next != NULL)
        {
            node *tmp = table[i]->next;
            free (table[i]);
            table[i] = tmp;
        }
        free (table[i]);
    }

    return true;
}




/** Problem log:
 * - Whitespace also in the dict word
 * - NEXT is still null after assigning something to word
 *
 **/