// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26


// Represents a node in a hash table
typedef struct node {
    char word[LENGTH + 1];
    struct node *next;
}
        node;


// Represents a hash table
typedef struct Hashtable {
    size_t size;
    node *table[N];
}
        Hashtable;


Hashtable *hashtable = NULL;


// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word) {
    return tolower(word[0]) - 'a';
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary) {
    // Initialize hash table
    hashtable = malloc(sizeof(Hashtable));
    hashtable->size = 0;
    for (int i = 0; i < N; i++) {
        hashtable->table[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL) {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF) {
        unsigned int word_hash = hash(word);

        node *new_node = malloc(sizeof(node));

        // Copy word from buffer to new node
        for (int i = 0; i < LENGTH + 1; i++) {
            new_node->word[i] = word[i];
        }

        // Point new node to whatever hashtable was pointing at
        new_node->next = hashtable->table[word_hash];
        // Point hashtable to new node
        hashtable->table[word_hash] = new_node;

        hashtable->size++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void) {
    if (hashtable != NULL) {
        return hashtable->size;
    }
    return 0;
}


// Returns true if word is in dictionary else false
bool check(const char *word) {
    unsigned int word_hash = hash(word);

    for (node *ptr = hashtable->table[word_hash]; ptr != NULL; ptr = ptr->next) {
        for (int i = 0; i < LENGTH + 1; i++) {
            if (tolower(word[i]) == ptr->word[i]) {
                if (word[i] == '\0') {
                    return true;
                }
            } else {
                break;
            }
        }
    }

    return false;
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void) {
    if (hashtable != NULL) {
        for (int i = 0; i < N; i++) {
            node *next_node = hashtable->table[i];
            hashtable->table[i] = NULL;
            while (next_node != NULL) {
                node *this_node = next_node;
                next_node = this_node->next;
                free(this_node);
            }
        }
        free(hashtable);
    }
    return true;
}
