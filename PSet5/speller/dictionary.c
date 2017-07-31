/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"

// define data structure for value-pointer pair
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

// create array of pointer to nodes
node* hashtable[HASHTABLE_SIZE];

// global u_int variable for dictionary tracking
unsigned int word_count = 0;

// global boolean for dictionary load/unload tracking
bool loaded = false;

// hash function
int hashIt(char* toHash)
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(toHash); i<n; i++)
        hash = (hash << 2) ^ toHash[i];
    return hash % HASHTABLE_SIZE;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    // create char array to store copy of word
    int len = strlen(word);
    char copyWord[len + 1];
    
    // changes word to lowercase and stores it in copyWord
    for (int i = 0; i < len; i++)
    {
       copyWord[i] = tolower(word[i]);
    }
    
    // add null terminator to end of char array
    copyWord[len] = '\0';
    
    // get buckey value
    int h = hashIt(copyWord);
    
    // point cursor node to first node of bucket
    node* cursor = hashtable[h];
    
    // check until the end of list
    while (cursor != NULL)
    {
        if (strcmp(cursor->word, copyWord) == 0)
        {
            // word is in dictionary
            return true;
        }
        else
        {
            // check next node
            cursor = cursor->next;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
    // NULL all elements in hashtable
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        hashtable[i] = NULL;
    }
    
    // open dictionary
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Unable to open dictionary\n");
        return false;
    }

    while (true)
    {
        // for every new word allocate memory to the node
        node* new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            printf("Unable to allocate memory to a node\n");
            return false;
        }
        
        // read a word from the dictionary and store it in new_node->word
        fscanf(fp, "%s", new_node->word);
        new_node->next = NULL;
        
        if (feof(fp))
        {
            // reach end of the file
            free(new_node);
            break;
        }

        word_count++;
        
        // hashtable[h] is the pointer to the key-value pair
        int h = hashIt(new_node->word);
        node* head = hashtable[h];
        
        // if bucket is empty, insert first node
        if (head == NULL)
        {
            hashtable[h] = new_node;
        }
        
        // if bucket not empty insert node to front of list
        else
        {
            new_node->next = hashtable[h];
            hashtable[h] = new_node;
        }
    }
    // close dictionary
    fclose(fp);
    loaded = true;
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    if (loaded)
    {
        return word_count;
    }
    else
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        node* cursor = hashtable[i];
        while (cursor != NULL)
        {
            // maintain connection to linked list using temp
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    loaded = false;
    return true;
}
