// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dictionary.h"
#include <strings.h>
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// global variable to hold word count
unsigned int wordCount = 0;


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int hashIndex = hash(word);
    node *cursor = table[hashIndex];

    // traverse through list
    while(cursor != NULL){

        // if we found a word
        if(strcasecmp(cursor->word,word) == 0){
            return true;
        }

        cursor = cursor->next;
    }
    return false;
}


// Hashes word to a number
unsigned int hash(const char *word)
{
    int length = strlen(word);
    char copy[length + 1];
    strcpy(copy, word);

    for(int i = 0; i < length; i++)
    {
        copy[i] = tolower(copy[i]);
    }

    char *wordCopy = copy;

    // implemented Dan's Bernstein's (djb2) algorithm
    // http://www.cse.yorku.ca/~oz/hash.html
    unsigned int hash = 5381;

    while(*wordCopy != 0)
    {
        hash = ((hash << 5) + hash) + *(wordCopy++);
    }

    return hash % N;
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open file
    FILE* dict = fopen(dictionary, "r");
    if(dict == NULL)
    {
        printf("File could not be opened\n");
        return false;
    }

    // initialize all buckets in hashtable to null
    for(int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // temp string that will hold dict word
    char dictStr[LENGTH+1];

    // iterate through every word in text file
    while (fscanf(dict, "%s", dictStr) != EOF)
    {
        // printf("%s\n",dictStr);

        // create new node
        node *newNode = malloc(sizeof(node));
        if (newNode == NULL)
        {
            printf("could not allocate memory");
            free(newNode);
            return false;
        }

        // assign fields to new node
        strcpy(newNode->word, dictStr);
        newNode->next = NULL;

        // hash word and get index
        int hashIndex = hash(dictStr);
        wordCount++;

        // if first node in bucket
        if (table[hashIndex] == NULL)
        {
            table[hashIndex] = newNode;
        }
        // else if there is more than one node in the bucket
        else
        {
            newNode->next = table[hashIndex];
            table[hashIndex] = newNode;
        }

        // break if we reached end of file
        if (feof(dict))
        {
            break;
        }
    }
    fclose(dict);
    return true;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordCount;
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // free memory a bucket at a time
    for(int i = 0; i < N; i++){

        node* cursor = table[i];

        while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    return true;
}
