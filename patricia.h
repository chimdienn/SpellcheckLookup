/*-------------------------------------------------------------- 
..Project: Assignment 2
  list.h :  
          = the interface of module patricia tree of the project
  NOTES:
          - this module is polymorphic
          - the linked list component is inherited from list.h module
----------------------------------------------------------------*/

#ifndef _PATRICIA_H_
#define _PATRICIA_H_

// Data definitions

typedef struct tree tree_t;

struct presult {
    char *key; // Key name
    void *data; // Pointer to matching records
    int matches_count; // Number of matching records
    int bits_compared;
    int node_accessed;
    int string_compared;
}; 
typedef struct presult presult_t;// presult is short for patricia_result



// Function definitions

/* Allocates new memory to hold the numBits specified and fills the allocated
    memory with the numBits specified starting from the startBit of the oldKey
    array of bytes. Adopted from Assignment 2 instruction */
char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits);

/* Returns the edit distance of two strings
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int editDistance(char *str1, char *str2, int n, int m);

// Create and return an empty patricia tree
tree_t *create_tree();

// Insert a new data to the patricia key, use linked list to store duplicates
void insert_tree(tree_t *tree, void *data, char* (*get_key)(void *));

// Find matching data in patricia tree with given key, return matching result
presult_t *search_tree(FILE *f, tree_t *tree, char *key, char* (*get_key)(void *), void (*print_data)(FILE *, void *));

// Free the tree, with the help of function "free_data" for freeing data component
void free_tree(tree_t *tree, void (*free_data)(void *));

#endif

