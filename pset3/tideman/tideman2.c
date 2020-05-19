

// Creating an identical array w/ margin, which does the exact same thing, but is never used for printing

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

typedef struct
{
    int winner;
    int loser;
    int margin;
}
pair2;


// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];
pair2 pairs2[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference; Array with length of cand count
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO

    // Check if name exists via Linary Search

    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
        // Record ranks in ranks array ; Ranks [0] is the index of the candidate who is the first preference
        // We assign the index from candidates to the respective rank in ranks
            ranks[rank] = i;
        // We get e.g. Index 0 assigned with 3 -> This means that Pref No 1 is 4th candidate in the candidates array
            return true;
        }
    }

    // If true wasn't yet returned

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO

    // candidates[] is the array with the index of each votable person

    // We always make as many entries in the global preferences as total preferences we have -> n * n*0.5 + n*0.5
    // Where n are the total candidates
    // preferences [i] [j] - No of people who prefer i over j -> We start with assigning all 1's for i (The voters first pref)


    // preferences [ranks[0]] [j] -> How we would iterate through all prefs of the first rank; D
    // Double loop to iterate through the ranks & then through assigning the e.g. 3 prefs for that rank



    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences [ranks[i]] [ranks[j]]++;
        }

    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO

    // "Battle" two candidates and determine who is preferred
    // We can only get e.g. 3 preferences for 3 candidates (1 vs 2; 1 vs 3; 2 vs 3) ;; 3*2 / 2
    // We only update pair count if a pair is added

    int index = 0;

    for (int i = 0; i < candidate_count; i++)
    {

    // We need two for loops since we have a 2D Array and need to iterate through both digits

    // Problem: We will double iterate some pairs: 0 1 vs 1 0 ; 0 2 vs 2 0 ; 0 3 vs 3 0;; 1 1 vs 1 1; 1 2 vs 2 1; 1 3 vs 3 1; 2 1 vs 1 2

        for (int j = 1; j < candidate_count; j++)
        {
            // If No of people preferring i over j is greater than preffering j over i

            if (preferences [i][j] > preferences[j][i])
            {
                pairs[index].winner = i;
                pairs[index].loser = j;
                
                pairs2[index].winner = i;
                pairs2[index].loser = j;
                pairs2[index].margin = preferences [i][j] - preferences[j][i];

                pair_count++;

            }
            else if (preferences [i][j] < preferences [j][i])
            {
                pairs[index].winner = j;
                pairs[index].loser = i;
                
                pairs2[index].winner = j;
                pairs2[index].loser = i;
                pairs2[index].margin = preferences [j][i] - preferences[i][j];

                pair_count++;
            }
        index++;
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO


    // Decreasing order of strength of victory -> How many people prefer the winner in that pair btw winner & loser
    // -> Take preferences array again to count the total preferences for the winner vs the loser
    // Check if two adjacent winners are the same if yes leave, if no swap them -- But then how do I sort them?

    // Recursion / Halving sort ;; Seems like too much

    // Bubble Sort w/ condition

    // WRONG SORTING -> Do not put all winners next to each other, but sort by margin of victory

    int swap_counter = -1;

    while (swap_counter != 0)
    {
        swap_counter = 0;

        // Double Array to go through all pairs with j & go through the preferences with i

        // We need to get the margin -> hence look through preferences once more and only switch
        // if the margin of lower than the one to the right

            for (int i = 0; i < pair_count; i++)
            {
                if (pairs2[i].margin < pairs2[i + 1].margin)
                {
                    int placeholder_w = pairs[i].winner;
                    int placeholder_l = pairs[i].loser;
                    
                    pairs[i].winner = pairs[i + 1].winner;
                    pairs[i].loser = pairs[i + 1].loser;
                    
                    pairs[i + 1].winner = placeholder_w;
                    pairs[i + 1].loser = placeholder_l;
                    
                    
                    int placeholder_w2 = pairs2[i].winner;
                    int placeholder_l2 = pairs2[i].loser;
                    int placeholder_m2 = pairs2[i].margin;

                    pairs2[i].winner = pairs2[i + 1].winner;
                    pairs2[i].loser = pairs2[i + 1].loser;
                    pairs2[i].margin = pairs2[i + 1].margin;

                    pairs2[i + 1].winner = placeholder_w2;
                    pairs2[i + 1].loser = placeholder_l2;
                    pairs2[i + 1].margin = placeholder_m2;

                    swap_counter++;

                }
            }
    }



    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO

    // Implement locked pairs
    // Implement an if condition to not add an arrow if it results in a cycle, ergo,
    // the person giving true on the next and so on and then back to him self
    // Need to start with the last element from sorted pairs pairs[]

    int candidate_array[candidate_count];


    for (int i = 0; i < candidate_count; i++)
    {
        candidate_array[i] = i;
    }

    int rem_can_count = candidate_count;

    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;

        for (int j = 0; j < candidate_count; j++)
        {
            if (pairs[i].loser == candidate_array[j])
            {
                rem_can_count--;
                candidate_array[j] = -1;
            }
            if (rem_can_count == 1)
            {
                for(int x = 0; x < candidate_count; x++)
                {
                    if(candidate_array[x] >= 0)
                    {
                        printf("%s\n", candidates[candidate_array[x]]);
                        return;
                    }
                }
            }
        }
    }




    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

