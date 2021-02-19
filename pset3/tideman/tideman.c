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

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

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
        // ranks[i] is voter's ith preference
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
    for (; rank < candidate_count; rank++) // Start at rank 0, each time a match is found add 1 to the rank interger
    {
        for (int i = 0; i < candidate_count; i++)
        {
            if (strcmp (name , candidates[i]) == 0) // Match found
            {
                ranks[rank] = i; // Asign rank to candidate
                rank++;
                return true;
            }
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int k = 0; k < (candidate_count - 1); k++)
    {
        for (int m = (k + 1); m < candidate_count; m++)
        {
            int i = ranks[k];
            int j = ranks[m];
            preferences[i][j]++; //add 1 to the preference counter of said pair
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int k = 0;
    for (int i = 0; i < (candidate_count - 1); i++)
    {
        for (int j = (i + 1); j <= candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[k].winner = i; // i is prefered over j
                pairs[k].loser = j;
                k++;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[k].winner = j; // j is prefered over i
                pairs[k].loser = i;
                k++;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            // Variables for current highest value, until compared, the first value is the highest
            int a = pairs[i].winner;
            int b = pairs[i].loser;
            int ab = preferences[a][b] - preferences[b][a];

            // Variables for value to be compared
            int c = pairs[j].winner;
            int d = pairs[j].loser;
            int cd = preferences[c][d] - preferences[d][c];

            //printf("ab:%i & cd:%i \n", ab, cd);

            if ( ab < cd)
            {
                pair temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Set to false a candidate over itself
    for (int i = 0; i < candidate_count; i++)
    {
        locked[i][i] = false;
    }

    // Start locking from the highest pair
    for (int i = 0; i < pair_count; i++)
    {
        int w = pairs[i].winner;
        int l = pairs[i].loser;
        int t = 0;

        // Check if opposite is already locked
        for (int j = 0; j < pair_count; j++)
        {
            if (locked[l][j] == true && locked[j][w] == true)
            {
                t++;
            }
        }

        // Check if final pair would create a cycle
        for (int j = 0; j < pair_count; j++)
        {
            if (i == (pair_count - 1) && locked[l][j] == true)
            {
                t++;
            }
        }
        if (t == 0)
        {
        locked[w][l] = true;
        locked[l][w] = false;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int t = 0;

        for (int j = 0; j < candidate_count; j++)
        {
                if (locked[j][i] == false)
                {
                 t++;
                }
        }

        if (t == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}
