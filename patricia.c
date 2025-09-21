/*-------------------------------------------------------------- 
..Project: Assignment 2
  list.c :  
          = the implementation of module patricia tree of the project
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "patricia.h"

typedef struct pnode pnode_t; // pnode is short for patricia_node
struct pnode {
    int prefix_bits;
    char *key;
    pnode_t *left;
    pnode_t *right;
    pnode_t *parent;
    list_t *list; // Store duplicated data in a linked list
    int num_records; // Number of data records
};


struct tree {
    pnode_t *root;
};

/* Helper function. Gets the bit at bitIndex from the string s. 
   Adopted from Assignment 2 instruction */
static int getBit(char *s, unsigned int bitIndex){
    assert(s && bitIndex >= 0);
    unsigned int byte = bitIndex / BITS_PER_BYTE;
    unsigned int indexFromLeft = bitIndex % BITS_PER_BYTE;
    /* 
        Since we split from the highest order bit first, the bit we are interested
        will be the highest order bit, rather than a bit that occurs at the end of the
        number. 
    */
    unsigned int offset = (BITS_PER_BYTE - (indexFromLeft) - 1) % BITS_PER_BYTE;
    unsigned char byteOfInterest = s[byte];
    unsigned int offsetMask = (1 << offset);
    unsigned int maskedByte = (byteOfInterest & offsetMask);
    /*
        The masked byte will still have the bit in its original position, to return
        either 0 or 1, we need to move the bit to the lowest order bit in the number.
    */
    unsigned int bitOnly = maskedByte >> offset;
    return bitOnly;
}

/* Allocates new memory to hold the numBits specified and fills the allocated
    memory with the numBits specified starting from the startBit of the oldKey
    array of bytes. Adopted from Assignment 2 instruction */
char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits){
    assert(numBits > 0 && startBit >= 0 && oldKey);
    int extraBytes = 0;
    if((numBits % BITS_PER_BYTE) > 0){
        extraBytes = 1;
    }
    int totalBytes = (numBits / BITS_PER_BYTE) + extraBytes;
    char *newStem = malloc(sizeof(char) * totalBytes);
    assert(newStem);
    for(unsigned int i = 0; i < totalBytes; i++){
        newStem[i] = 0;
    }
    for(unsigned int i = 0; i < numBits; i++){
        unsigned int indexFromLeft = i % BITS_PER_BYTE;
        unsigned int offset = (BITS_PER_BYTE - indexFromLeft - 1) % BITS_PER_BYTE;
        unsigned int bitMaskForPosition = 1 << offset;
        unsigned int bitValueAtPosition = getBit(oldKey, startBit + i);
        unsigned int byteInNewStem = i / BITS_PER_BYTE;
        newStem[byteInNewStem] |= bitMaskForPosition * bitValueAtPosition;
    }
    return newStem;
}

/* Returns min of 3 integers 
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
static int min(int a, int b, int c) {
    if (a < b) {
        if(a < c) {
            return a;
        } else {
            return c;
        }
    } else {
        if(b < c) {
            return b;
        } else {
            return c;
        }
    }
}

/* Returns the edit distance of two strings
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int editDistance(char *str1, char *str2, int n, int m){
    assert(m >= 0 && n >= 0 && (str1 || m == 0) && (str2 || n == 0));
    // Declare a 2D array to store the dynamic programming
    // table
    int dp[n + 1][m + 1];

    // Initialize the dp table
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            // If the first string is empty, the only option
            // is to insert all characters of the second
            // string
            if (i == 0) {
                dp[i][j] = j;
            }
            // If the second string is empty, the only
            // option is to remove all characters of the
            // first string
            else if (j == 0) {
                dp[i][j] = i;
            }
            // If the last characters are the same, no
            // modification is necessary to the string.
            else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = min(1 + dp[i - 1][j], 1 + dp[i][j - 1],
                    dp[i - 1][j - 1]);
            }
            // If the last characters are different,
            // consider all three operations and find the
            // minimum
            else {
                dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1],
                    dp[i - 1][j - 1]);
            }
        }
    }

    // Return the result from the dynamic programming table
    return dp[n][m];
}

// Create and return an empty patricia tree
tree_t *create_tree() {
    tree_t *tree = malloc(sizeof(tree_t));
    assert(tree);
    tree->root = NULL; 
    return tree;
}

// Initialize and return a new pnode with given key and data
static pnode_t *create_pnode(int prefix_bits, char *key, void *data) {
    pnode_t *new = malloc(sizeof(pnode_t));
    assert(new);
    
    // Initialize the fields
    new->prefix_bits = prefix_bits;
    new->key = key;
    new->left = NULL;
    new->right = NULL;
    new->parent = NULL;

    // If data is provided, create a linked list and append data to it
    if (data != NULL){
        new->list = create_list();
        append_list(new->list, data);
        new->num_records = 1;
    }
    else {
        new->list = NULL;
        new->num_records = 0;
    }

    return new;
}

// Compare two strings bit by bit up to prefix_bits
// Return mismatched bit index
static int compare_bits(int prefix_bits, char *str1, char *str2){
    int i;
    for (i=0; i < prefix_bits; i++){
        if (getBit(str1, i) != getBit(str2, i)) return i; // Mismatch at index i
    }
    return i; // Strings matched
}

// Initialize a new pnode to hold data 
// Add the pnode to patricia tree using recursion
static pnode_t *recursive_insert_pnode(pnode_t *root, int prefix_bits, char *data_key, void *data) {
    if (root == NULL) {
        // Create and return a new pnode
        pnode_t *new = create_pnode(prefix_bits, data_key, data);
        return new;
    } 

    else {
        // Find bit index where mismatch occurs
        int new_prefix = compare_bits(root->prefix_bits, root->key, data_key);
        
        // If the prefix matches completely
        if (new_prefix == root-> prefix_bits) {
            // If the prefix also matches the prefix_bits of the new key
            if (new_prefix == prefix_bits){
                free(data_key); // Free the data_key as no longer needed
                append_list(root->list, data); // Add the new data to the pnode's list
                root->num_records +=1;
                return root;
            }
            else {
                // Create a new data_key stem with the remaining prefix bits
                char *new_data_key = createStem(data_key, new_prefix, prefix_bits - new_prefix);
                free(data_key);
                data_key = new_data_key;

                // Recursively insert into the correct subtree based on the first bit of the new key
                if (getBit(data_key, 0) == 1) {
                    root->right = recursive_insert_pnode(root->right, prefix_bits - new_prefix, data_key, data);
                } else {
                    root->left = recursive_insert_pnode(root->left, prefix_bits - new_prefix, data_key, data);
                }
            }
            
        } 
        // Mismatch between root key and data_key, restructure the tree
        else {
            // Create a new key for the data using the remaining prefix
            char *new_data_key = createStem(data_key, new_prefix, prefix_bits - new_prefix);
            free(data_key);
            data_key = new_data_key;

            // Create a new key for the root pnode 
            char *new_root_key = createStem(root->key, 0, new_prefix);
            char *remaining_root_key = createStem(root->key, new_prefix, root->prefix_bits - new_prefix);
            free(root->key);
            root->key = remaining_root_key;
            root->prefix_bits = root->prefix_bits - new_prefix;

            // Create a new root pnode with the common prefix
            pnode_t *new_root = create_pnode(new_prefix, new_root_key, NULL);

            // Create a new pnode for the data being inserted
            pnode_t *data_pnode = create_pnode(prefix_bits - new_prefix, data_key, data);

            // Update parent pointers for the new and existing nodes
            new_root->parent = root->parent;
            root->parent = new_root;
            data_pnode->parent = new_root;

            // Insert the new pnodes into the correct branch based on the first bit of the new key
            if (getBit(data_key, 0) == 1) {
                    new_root->left = root;
                    new_root->right = data_pnode;
                } 
            else {
                    new_root->left = data_pnode;
                    new_root->right = root;
                }
            return new_root;
        }
        return root;     
    }
}

// Insert a data to the patricia key, with the helper function "get_key" to get the key from data
void insert_tree(tree_t *tree, void *data, char* (*get_key)(void *)){
    char *data_key = strdup(get_key(data)); // Allocate memory for key
    int prefix_bits = (strlen(data_key)+ 1) * BITS_PER_BYTE; // Calculate key's bits, including null byte
    tree->root = recursive_insert_pnode(tree->root, prefix_bits, data_key, data);
}

// Traverse the tree and add all leaf pnodes to a linked list
// Implement recursion and in-order traversal logic
static void add_leaves_in_order(pnode_t* root, list_t *list) {
    if (root == NULL) {
        return;
    }

    // Traverse the left subtree
    add_leaves_in_order(root->left, list);

    if (root->right == NULL && root->left==NULL){
        // Leaf pnode found
        append_list(list, root);
    }

    // Traverse the right subtree
    add_leaves_in_order(root->right, list);
}

// Spellchecking process 
// Find the closest string when mismatch occur using edit distance
static void handle_mismatch(pnode_t *root, presult_t *presult, char* (*get_key)(void *)){
    // Create a linked list of descendants
    list_t* candidates_list = create_list();
    add_leaves_in_order(root, candidates_list); 

    int min_distance;
    pnode_t *closest_candidate;

    node_t *current = candidates_list->head;

    // Iteratively calculate the edit distance of each candidate and update the closet candidate
    while (current != NULL){
        char *candidate_key = get_key(((pnode_t *)(current->data))->list->head->data);

        // Calcuate edit distance of current candidate
        int candidate_distance = editDistance(presult->key, candidate_key, strlen(presult->key), strlen(candidate_key));

        if (current == candidates_list->head){
            closest_candidate = current->data;
            min_distance = candidate_distance;
        }
        else {
            if (candidate_distance < min_distance){
                // New closest candidate found
                closest_candidate = current->data;
                min_distance = candidate_distance;
            }
        }
        current = current->next;
    }

    // Update the presult with the closest candidate
    presult->data = closest_candidate->list;
    presult->matches_count = closest_candidate->num_records;

    // Free candidate list after use, but without freeing the pnode data inside
    free_list_keep_data(candidates_list);
}


// Recursively search for a key in the tree
// Implement spellchecking process for mismatch, print the maching data to output file
static void recursive_search_pnode(FILE *f, presult_t *presult, pnode_t *root, int prefix_bits, char *current_key, char* (*get_key)(void *), void (*print_data)(FILE *, void *)){
    if (root == NULL){
        // Tree is empty, key not found
        free(current_key);
        fprintf(f, "%s -->\n", presult->key);
        return;
    }

    // If both data and root are left with null byte, it must be a match
    if (prefix_bits < BITS_PER_BYTE && root->prefix_bits == prefix_bits){
        free(current_key);

        // Update the presult with the matched data
        presult->data = root->list;
        presult->matches_count = root->num_records;

        // Print the data to output file
        fprintf(f, "%s -->\n", presult->key);
        print_list_to_file(f, presult->data, print_data);
        return;
    }

    // Else if data is left with null byte, it must be a mismatch
    else if (prefix_bits < BITS_PER_BYTE){
        free(current_key);
        // Find the closest data by edit distance
        handle_mismatch(root->parent, presult, get_key);

        // Print the data to output file
        fprintf(f, "%s -->\n", presult->key);
        print_list_to_file(f, presult->data, print_data);
        return;
    }

    presult->node_accessed +=1;
    
    // Compare bits between the root's key and the current key being searched
    int bits_compared = compare_bits(root->prefix_bits, root->key, current_key);

    // If two keys matched
    if (bits_compared == root->prefix_bits) {
        presult->bits_compared += bits_compared;

        // If the compared bits match the prefix length, data found
        if (bits_compared == prefix_bits){
            free(current_key);

            // Update the presult with the matched data
            presult->data = root->list;
            presult->matches_count = root->num_records;

            // Print the data to output file
            fprintf(f, "%s -->\n", presult->key);
            print_list_to_file(f, presult->data, print_data);
            return;
        }
        else {
            // Create a new key with the remaining bits
            char *new_key = createStem(current_key, bits_compared, prefix_bits - bits_compared);
            free(current_key);
            current_key = new_key;

            // Recursively search either subtree based on the next bit
            if (getBit(current_key, 0) == 1) {
                recursive_search_pnode(f, presult, root->right, prefix_bits - bits_compared, current_key, get_key, print_data);
            } else {
                recursive_search_pnode(f, presult, root->left, prefix_bits - bits_compared, current_key, get_key, print_data);
            }
        }
    }
    // Mismatch occur
    else {
        free(current_key);
        presult->bits_compared += (bits_compared + 1);
        // Find the closest data by edit distance
        handle_mismatch(root, presult, get_key);

        // Print the data to output file
        fprintf(f, "%s -->\n", presult->key);
        print_list_to_file(f, presult->data, print_data);
        return;
    }
}


// Search data in patricia by given key, implement spellchecking logic when mismatch occurs
// Print matching data to the output file, return matching result
presult_t *search_tree(FILE *f, tree_t *tree, char *key, char* (*get_key)(void *), void (*print_data)(FILE *, void *)) {
    assert(tree);
    presult_t *presult = malloc(sizeof(*presult)); // Allocate memory for presult
    assert(presult);

    presult->key = key;
    char * current_key = strdup(key);
    assert(current_key);

    presult->data = NULL;
    presult->bits_compared = 0;
    presult->node_accessed = 0;
    
    // Calculate total bits of the key, including null byte
    int prefix_bits = (strlen(current_key)+ 1) * BITS_PER_BYTE;
    recursive_search_pnode(f, presult, tree->root, prefix_bits, current_key, get_key, print_data);

    presult->string_compared = 1; // Height of the tree is one-string long

    return presult;
}

// Visit and free every pnode using recusrion
// With the help of function "free_data" for freeing data component
static void recursive_free_pnode(pnode_t *root, void (*free_data)(void *)){
    if (root == NULL) return;
    recursive_free_pnode(root->left, free_data);
    recursive_free_pnode(root->right, free_data);

    free(root->key);
    if (root->right == NULL && root->left == NULL){
        // Leaf pnode, contains linked-listed data
        free_list(root->list, free_data);
    }
    free(root);
}

// Free the tree, with the help of function "free_data" for freeing data component
void free_tree(tree_t *tree, void (*free_data)(void *)){
    recursive_free_pnode(tree->root, free_data);
    free(tree);
}


