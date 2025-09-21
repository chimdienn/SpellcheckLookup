/*-------------------------------------------------------------- 
..Project: Assignment 2
  data.h :  
          = the interface of module data of the project
----------------------------------------------------------------*/

#ifndef _DATA_H_
#define _DATA_H_

#define MAX_FIELD_LEN 127
#define MAX_LINE_LEN 511

// Data definitions

typedef struct suburb suburb_t;

// Function definitions

// Read a single record from input file, return a struct of suburb details
suburb_t *read_suburb(FILE *f);

// Retrieve the name of the suburb from a suburb_t struct
char *get_suburb_name(void *s);

// Print detailed suburb information to output file
void print_suburb_detail(FILE *f, void *s);

// Print surburb details in csv format to output file
void print_suburb_to_csv(FILE *f, void *s);

// Free the memory allocated for the struct and its dynamically allocated fields
void free_suburb(void *s);

#endif
