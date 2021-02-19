// Implements a dictionary's functionality
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 32;

// Hash table
node *table[N];

// Count words in dictionary
int count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Hash word
    int index = hash(word);

    // Go to table[hash1]
    node *tempcheck = table[index];

    // Compare with dictionary
    while (tempcheck != NULL)
    {
        int s = strcasecmp(word, tempcheck->word);
        if (s == 0) // Word is in dictionary
        {
            return true;
        }
        else tempcheck = tempcheck->next;
    }

    // Word isn't in dictionary
    return false;
}

// Hashes word to a number - djb2 hash function by Dan Bernstein
unsigned int hash(const char *word)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *word++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Preset every table header to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open dictionary
    FILE *dic = dic = fopen(dictionary,"r");
    if (dic == NULL)
    {
        printf("Dictionary unreadable\n");
        return 1;
    }

    // Read strings from file on a loop
    char *dword = malloc(LENGTH + 1);
    node *tempload = NULL;


    while (fscanf(dic, "%s",dword) == 1)
    {
        // Hash word so it returns a unique value
        int value = hash(dword);

        // Create a node for each word
        tempload = malloc(sizeof(node));
        if (tempload == NULL)
        {
            printf("failed to malloc tempload!\n");
            return false;
        }

        strcpy(tempload->word, dword);
        tempload->next = NULL;

        // Point temp to the same as head
        if (table[value] == NULL)
        {
            table[value] = tempload;
        }
        else if (table[value] != NULL)
        {
            tempload->next = table[value];

            // Point head to temp
            table[value] = tempload;
        }
        // Update word count of dictionary
        count++;
    }
    free(dword);
    fclose(dic);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Dictionary not loaded
    if (count == 0)
    {
       printf("Dictionary not loaded!\n");
       return 0;
    }
    else return count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // CREATE ARROW ONLY ONCE
    node *arrow = NULL;

    // Start from table 0 until last table
    for (int i = 0; i < N; i++)
    {
        arrow = table[i];

        // Table header has content
        if (arrow != NULL)
        {
            do
            {
                // Create a temp node that equals arrow
                node *temp = arrow;

                // Point arrow to next value to avoid loosing data
                arrow = arrow->next;

                // Clear data
                free(temp);

            } while (arrow != NULL);
        }

        // Header is empty
        else
        {
            node *temp = arrow;
            free(temp);
        }
    }
    // Clear malloc
    free(arrow);
    return true;    
}

