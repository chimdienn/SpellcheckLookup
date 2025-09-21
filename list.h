/*-------------------------------------------------------------- 
..Project: Assignment 2
  list.h :  
          = the interface of module linked list of the project
  NOTES:
          - this module is polymorphic
          - the data component in a list node is of type void*
----------------------------------------------------------------*/

#ifndef _LIST_H_
#define _LIST_H_

#define INITIAL_RESULT_SIZE 100
#define STRING_MATCHED 1
#define STRING_UNMATCHED -1
#define BITS_PER_BYTE 8

// Data definitions

typedef struct node node_t;
struct node {
    void *data; // Data has type "void *" for polymorphic use
    node_t *next; // Point to the next node
};

struct list {
    node_t *head; // First node of the list
    node_t *tail; // Last node of the list
};
typedef struct list list_t;

struct result {
    char *key; // Key name
    void **records; // Array of pointers poiting to matching records
    int matches_count; // Number of matching records
    int bits_compared;
    int node_accessed;
    int string_compared;
};
typedef struct result result_t;

// Function definitions

// Create and return an empty linked list
list_t *create_list();

// Insert a new node with value "data" to the end of list 
void append_list(list_t *list, void *data);

// Find matching nodes in linked list with given key, return matching result
result_t *search_node(FILE *f, list_t *list, char *key, 
                      char* (*get_data)(void *), void (*print_data)(FILE *, void *));

// Delete matching nodes in linked list with given key, return number of deletions
int delete_node(list_t *list, char *key, char* (*get_data)(void *), void (*free_data)(void *));

// Print the data of list to output file f, with the help of function "print_data"
void print_list_to_file(FILE *f, list_t *list, void (*print_data)(FILE *, void *));

// Free the memory allocated to a result "r"
void free_result(void *r);

// Free the list, with the help of function "free_data" for freeing data component
void free_list(list_t *list, void (*free_data)(void *));

// Free the list and node pointers without freeing data
void free_list_keep_data(list_t *list);

#endif

