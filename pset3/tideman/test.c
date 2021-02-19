#include <cs50.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    int winner;
    int loser;
}
pair;

pair pairs[9];
int pair_count;

void sort_pairs(void);
int input(void);

int main(void)
{
    input();

    while (pair_count > 9)
    {
        printf("invalid value\n");
        input();
    }

    for (int i = 0; i < pair_count; i++)
    {
        pairs[i].df = get_int ("%i numbers: ", i + 1);
    }

    sort_pairs();

    for (int i = 0; i < pair_count; i++)
    {
        printf("%i position: %i\n", i + 1, pairs[i].df);
    }
}

void sort_pairs(void)
{
    pair high[pair_count];

    for (int i = 0; i < pair_count; i++)
    {
        high[i] = pairs[i];

        for (int j = i + 1; j < pair_count; j++)
        {
            // Variables for current highest value
            int w = high[i].winner;
            int l = high[i].loser;
            int df = preferences[w][l] - preferences[w][l];
            
            // Variables for value to be compared
            int a = pairs[j].winner;
            int b = pairs[j].loser;
            int ab = preferences[a][b] - preferences[b][a];
            
            if ( df < ab)
            {
                pair temp = high[i];
                high[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }

    for (int i = 0; i < pair_count; i++)
    {
        pairs[i] = high[i];
    }
    return;
}

int input(void)
{
    pair_count = get_int ("amount of numbers: ");
    return pair_count;
}