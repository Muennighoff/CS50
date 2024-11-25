#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    
    // Linear Search for name in candidate[i].name
    
    
    for (int i = 0; i < candidate_count; i++)
    {
         
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++; 
            return true;
        }
    }
    
    return false;
}


// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO
    
    // Loop over all votes and sort with highest vote at 0
    // Buuble sort with the biggest first
    
        int swap_counter = 0;
        do
        {
        swap_counter = 0;
        for (int i = 0; i < candidate_count; i++)
        {
            if (candidates[i].votes < candidates[i + 1].votes)
            {
                int placeholder_v = candidates[i].votes;
                string placeholder_n = candidates[i].name;
                candidates[i].votes = candidates[i + 1].votes;
                candidates[i].name = candidates[i + 1].name;
                candidates[i + 1].votes = placeholder_v; 
                candidates[i + 1].name = placeholder_n;
                swap_counter++;
            }
        }
        }
        while(swap_counter != 0);
    
    // Print the winners: For Loop to print multiple if applicable
    
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > candidates[i + 1].votes)
        {
            printf("%s\n", candidates[i].name);
            return;
        }
        else
        {
            printf("%s\n", candidates[i].name);
        }
    }
}

