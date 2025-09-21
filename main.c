/*---------------------------------------------------------------------------------
..Project: Assignment 2
  main.c : 
             = the main program of the project 
  Compiled by: Quy Trong Duc Tran ID: 1489523
  
  Note: The comment style for this project is adopted from project qStud W3.4
----------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "list.h"
#include "patricia.h"

// Functionality stages for dictionary
#define RETRIVAL 1
#define REMOVAL 2
#define SEARCH_COMPARISIONS 3
#define PATRICIA_TREE 4

char *read_header_line(FILE *f);
void print_header(FILE *f, char *header);

list_t *get_suburb_list(FILE *f);

list_t *lookup_suburb(int stage, char *output_file_name, list_t *suburb_dict);
void delete_suburb(char *output_file_name, list_t *suburb_dict, char *header);

tree_t *get_suburb_tree(FILE *f);
void lookup_tree(char *output_file_name, tree_t *suburb_tree);

int main(int argc, char *argv[]){
    assert(argc==4);
    int stage = atoi(argv[1]);

    FILE *f = fopen(argv[2], "r");
    assert(f);

    char *header = read_header_line(f); // Read header from input file
    
    // Linked list lookup stage
    if (stage == RETRIVAL || stage == SEARCH_COMPARISIONS){
        list_t *suburb_dict = get_suburb_list(f); // Build dictionary of suburbs
        fclose(f);
        list_t *result_list = lookup_suburb(stage, argv[3], suburb_dict);

        
        free_list(result_list, free_result); // Free result after use
        free_list(suburb_dict, free_suburb);
    }

    // Linked list removal stage
    else if (stage == REMOVAL){
        list_t *suburb_dict = get_suburb_list(f); // Build dictionary of suburbs
        fclose(f);
        delete_suburb(argv[3], suburb_dict, header);

        free_list(suburb_dict, free_suburb);
    }

    // Patricia lookup and spellchecker stage
    else if (stage == PATRICIA_TREE){
        tree_t * suburb_tree = get_suburb_tree(f); // Build patricia tree of suburbs
        fclose(f);

        lookup_tree(argv[3], suburb_tree);
        free_tree(suburb_tree, free_suburb);
        
    }
    free(header);
    return 0;
}

// Read the first line of file f
char *read_header_line(FILE *f) {
    assert(f);

    char temp[MAX_LINE_LEN + 1];
    int i = 0;
    char c;

    // Read characters until newline or EOF
    while ((c = fgetc(f)) != '\n' && c != EOF) {
        if (i < MAX_LINE_LEN) { 
            temp[i] = c;
            i++;
        }
    }
    temp[i] = '\0'; // Null-terminate the string

    char * header = malloc((i+1)* sizeof(char));
    assert(header);

    strcpy(header, temp);
    return header;
}

// Print header to file f
void print_header(FILE *f, char *header) {
    fprintf(f, "%s\n", header);
}


// Read suburbs from input file, return a linked list of suburbs
list_t *get_suburb_list(FILE *f){
    list_t *suburb_dict = create_list();

    // Build linked list of suburbs
    suburb_t *s;
    while((s = read_suburb(f))){
        append_list(suburb_dict, s);
    }

    return suburb_dict;
}



// Look up suburbs by names, print matching records to output_file_name
// Return a linked list of keys and matching records
list_t *lookup_suburb(int stage, char *output_file_name, list_t *suburb_dict){
    FILE *f = fopen(output_file_name, "w");
    assert(f);

    char *suburb_key=NULL;
    size_t buffer_size = 0;     // Size of the buffer

    list_t *result_list = create_list(); // Create a linked list to store result

    // Read each suburb name from standard input
    while (getline(&suburb_key, &buffer_size, stdin) != -1){
        result_t *result;

        size_t len = strlen(suburb_key);
        suburb_key[len-1] = '\0'; // Remove newline character at the end

        // Search for suburb in dictionary and print details
        result = search_node(f, suburb_dict, suburb_key, 
                             get_suburb_name, print_suburb_detail); 
        printf("%s --> ", result->key);
        if (result->matches_count > 0){
            if (stage == RETRIVAL){
                // Stage 1
                printf("%d records found\n", result->matches_count);
            }

            else if (stage == SEARCH_COMPARISIONS){
                // Stage 3
                printf("%d records - comparisons: b%d n%d s%d\n",
                        result->matches_count, result->bits_compared,
                        result->node_accessed, result->string_compared);
            }
        }
        else {
            printf("NOTFOUND\n");
        }

        append_list(result_list, result);
    }
    free(suburb_key); // Free the memory allocated by getline
    fclose(f);

    return result_list;
}


// Remove all records matching by names from stdin
// Output remaining records to output_file_name
void delete_suburb(char *output_file_name, list_t *suburb_dict, char *header){
    char *suburb_key = NULL;
    size_t buffer_size = 0;  // Size of the buffer

    int count; 

    // Read each suburb name from standard input
    while (getline(&suburb_key, &buffer_size, stdin) != -1){
        size_t len = strlen(suburb_key);
        suburb_key[len-1] = '\0'; // Remove newline character at the end

        count = delete_node(suburb_dict, suburb_key, get_suburb_name, free_suburb); 
        printf("%s --> ", suburb_key);
        if (count > 0){
            printf("%d records deleted\n", count);
        }
        else {
            printf("NOTFOUND\n");
        }
    }
    free(suburb_key);

    FILE *f = fopen(output_file_name, "w");
    assert(f);

    // Print header and remaining records to ouput file
    print_header(f, header);
    print_list_to_file(f, suburb_dict, print_suburb_to_csv);

    fclose(f);
}

// Read suburbs from input file, return a patricia tree of suburb keys
tree_t *get_suburb_tree(FILE *f){
    tree_t *suburb_tree = create_tree();

    // Build patricia tree of suburbs
    suburb_t *s;
    while((s = read_suburb(f))){
        insert_tree(suburb_tree, s, get_suburb_name);
    }

    return suburb_tree;
}

// Look up suburbs by names, implement spellchecking process 
// Print matching records to output_file_name
void lookup_tree(char *output_file_name, tree_t *suburb_tree){
    FILE *f = fopen(output_file_name, "w");
    assert(f);

    char *suburb_key=NULL;
    size_t buffer_size = 0;     // Size of the buffer

    // Read each suburb name from standard input
    while (getline(&suburb_key, &buffer_size, stdin) != -1){
        presult_t *presult;

        size_t len = strlen(suburb_key);
        suburb_key[len-1] = '\0'; // Remove newline character at the end

        // Search for the key in patricia tree
        presult = search_tree(f, suburb_tree, suburb_key, get_suburb_name, print_suburb_detail);

        printf("%s --> ", presult->key);
        if (presult->data != NULL){
            printf("%d records - comparisons: b%d n%d s%d\n",
            presult->matches_count, presult->bits_compared,
            presult->node_accessed, presult->string_compared);
        }
        else {
            printf("NOTFOUND\n"); 
        }
        free(presult); // Free presult after use
    }
    free(suburb_key); // Free the memory allocated by getline
    fclose(f);
}


