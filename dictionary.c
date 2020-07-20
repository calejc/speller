// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define HASHTABLE_SIZE 65536

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[HASHTABLE_SIZE];

// Hash function
int hash_it(char* needs_hashing)
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(needs_hashing); i<n; i++)
        hash = (hash << 2) ^ needs_hashing[i];
    return hash % HASHTABLE_SIZE;
}

// Declare a counter to determine dictionary size
unsigned int word_count = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {

        // Create new node pointer
        node* new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        // Copy word into node pointer
        strcpy(new_node->word, word);

        // Hash word, returning an index
        int index = hash_it(new_node->word);

        // If hashtable index is empty, insert
        if (hashtable[index] == NULL)
        {
            hashtable[index] = new_node;
            new_node->next = NULL;
        }

        // If hashtable index already has a value,
        // Insert at beginning of list
        else
        {
            new_node->next = hashtable[index];
            hashtable[index] = new_node;
        }

        word_count++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
    return 0;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{

    // Create a new pointer for the word, so we can use it. (const variables are read-only)
    int L = strlen(word);
    char word_copy[L + 1];

    // Change the word to lowercase so we don't erroneously flag correctly spelled words with capital letters.
    for (int i = 0; i < L; i++)
    {
        word_copy[i] = tolower(word[i]);
    }

    word_copy[L] = '\0';

    // Hash word returning the index
    int index = hash_it(word_copy);

    // If index is empty, return false.
    if (hashtable[index] == NULL)
    {
        return false;
    }

    // Create cursor to traverse through linked list
    node* cursor = hashtable[index];

    while (cursor != NULL)
    {
        if (strcmp(cursor->word, word_copy) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    // If the word is not in dictionary, return false.
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{

    // Iterate through hashtable
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        // Initiate a cursor to traverse the hashtable
        node* cursor = hashtable[i];

        // Loop while the hashtable[i] index has a value
        while (cursor != NULL)
        {
            // Initiate a temporary node
            node* temp = cursor;

            // Move the cursor to the next node in linked list
            cursor = cursor->next;

            // Free the temporary node
            free(temp);
        }

    }

    return true;
}
