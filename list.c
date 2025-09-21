/*-------------------------------------------------------------- 
..Project: Assignment 2
  list.c :  
          = the implementation of module linked list of the project
----------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"



// Create & return an empty linked list
// Adopted from project qStud
list_t *create_list(){
    list_t *list= malloc(sizeof(*list));
    assert(list);
    list->head = list->tail = NULL;
    return list;
}

// Inserts a new node with value "data" to the end of list
// Adopted from project qStud
void append_list(list_t *list, void *data) {
	assert(list);

	// Create a new node and set data 
	node_t *new = malloc(sizeof(*new));
	assert(new);
	new->data = data;
	new->next = NULL;
	if (list->head== NULL){
        // First node of the list
		list -> head = list -> tail = new;
	}

	else {
		list->tail->next = new;
		list->tail = new;
	}
}

// Compare two strings, update the bits and string compared
static int compare_string(result_t *result, char *str1, char *str2){
    result->string_compared += 1;

    int index = 0;
    while (str1[index] && str2[index]){
        if (str1[index] != str2[index]) {
            result->bits_compared += (index+1) * BITS_PER_BYTE;
            return STRING_UNMATCHED;
        }
        index++;
    }
    result->bits_compared += (index+1) * BITS_PER_BYTE;
    if (!str1[index] && !str2[index]) return STRING_MATCHED;
    return STRING_UNMATCHED;
}

// Find matching nodes in linked list with given key, print data to output file f 
// Return a struct of matching result, NULL if failure in memory allocation
result_t *search_node(FILE *f, list_t *list, char *key, char* (*get_data)(void *),
                      void (*print_data)(FILE *, void *)) {
	assert(list);

    result_t *result = malloc(sizeof(*result)); // Allocate memory for result
    assert(result);

    result->key = malloc((strlen(key)+1) * sizeof(char));
    assert(result->key);
    strcpy(result->key, key);

    // Allocate memory for storing pointers to matching records
    int size = INITIAL_RESULT_SIZE;
    result->records= (void **) malloc(size * sizeof(void *));
    assert(result->records);

	int matches_count = 0;
    result->bits_compared = 0;
    result->node_accessed = 0;
    result->string_compared = 0;
    
    node_t *current = list->head;

	fprintf(f, "%s -->\n", key); // Print search key to file

    while (current != NULL) {
        result->node_accessed+=1;

        if (compare_string(result, get_data(current->data), key) == STRING_MATCHED) {
            // Store the matching record and increment the count
            if (matches_count == size) {
                // Increase the size of the array
                size *= 2;
                result->records= (void **) realloc(result->records, size * sizeof(void *));
                assert(result->records);
            }
            result->records[matches_count] = current->data;
            matches_count++;

			print_data(f, current->data); // Print data to file
        }
        current = current->next;
    }

    if (matches_count == 0){
        // No matching records
        free(result->records);
        result->records = NULL;
    }

    else {
        // Resize down to fix the exact number of elements
        result->records = (void **) realloc(result->records, matches_count * sizeof(void *));
        assert(result->records);
    }

    result->matches_count = matches_count;

    return result;
}

// Delete matching nodes in linked list with given key
// Return number of deleted nodes
int delete_node(list_t *list, char *key, char* (*get_data)(void *), void (*free_data)(void *)) {
    assert(list);

    int count = 0;
    node_t *current = list->head;
    node_t *previous = NULL;

    while (current != NULL) {
        if (strcmp(get_data(current->data), key) == 0) {
            if (previous == NULL) {
                // Delete the head node
                list->head = current->next;
                if (list->head == NULL) {
                    // List is now empty
                    list->tail = NULL;
                }

            } else {
                // Bypass the node to delete
                previous->next = current->next;
                if (previous->next == NULL) {
                    // Delete the tail node
                    list->tail = previous;
                }
            }

            // Free the memory of the node to delete
            free_data(current->data);
            free(current);
            count++;

            // Update current to continue from the next node
            if (previous == NULL) {
                current = list->head;
            }
            else {
                current = previous->next;
            }

        } else {
            // No deletion, move to the next node
            previous = current;
            current = current->next;
        }
    }

    return count;
}

// Print the data of list to ouput file f, with the help of function "print_data" 
void print_list_to_file(FILE *f, list_t *list, void (*print_data)(FILE *, void *)){
	assert(list);

	node_t *current = list->head;

	while (current != NULL){
		print_data(f, current -> data); // Print the data
		current = current -> next;
	}
}

// Free the memory allocated for the result and its dynamically allocated fields
// Adopted from project qStud
void free_result(void *r){
    free(((result_t *)r)->key);
    if (((result_t *)r)->matches_count != 0) free(((result_t *)r)->records);
    free(r);
}

// Free the list, with the help of function "free_data" for freeing data component
// Adopted from project qStud
void free_list(list_t *list, void (*free_data)(void *)){
	assert(list);
	node_t *current = list->head; 
	while (current) {            
		node_t *tmp = current;
		current = current->next;   
		free_data(tmp->data);  // Free the data component
		free(tmp);   // Free the node
	}
	free(list);  // Free the list itself
}

// Free the list and node pointers without freeing data
void free_list_keep_data(list_t *list){
	assert(list);
	node_t *current = list->head; 
	while (current) {            
		node_t *tmp = current;
		current = current->next;   
		free(tmp);   // Free the node
	}
	free(list);  // Free the list itself
}



