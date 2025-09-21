/*-------------------------------------------------------------- 
..Project: Assignment 2
  data.c :  
          = the implementation of module data of the project
----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"

// Structure representing a suburb 
struct suburb {
    int comp_code;
    int code_suburb;
    char *name_suburb;
    int year;
    char *code_state;
    char *name_state;
    char *code_government;
    char *name_government;
    double latitude;
    double longitude;
};

// Read and handle field enclosed in double quotes from input file f
static void read_quoted_field(FILE *f, char *field_name) {
    // Read up to a comma, assuming no quotes are involved
    fscanf(f, "%[^,],", field_name);  
    
    // Check if the field starts with a quote
    if (field_name[0] == '\"') {
        // Remove the leading quote from the field
        int len = strlen(field_name);
        for (int i = 0; i < len-1; i++){
            field_name[i]= field_name[i+1];
        }
        field_name[len-1]= ',';
        field_name[len] = ' ';
        field_name[len+1] = '\0';

        char remaining[MAX_FIELD_LEN+1];
        fscanf(f, " %[^\"]\",", remaining);  // Read until the next quote and comma
    
        strcat(field_name, remaining);  // Append the remaining part
        
    }
}

// Read a single record from input file f
// Return a struct of suburb details or NULL if unsuccessful
suburb_t *read_suburb(FILE *f) {
	suburb_t *s= NULL;
	int comp_code, code_suburb, year;
    char name_suburb[MAX_FIELD_LEN+1];
    char code_state[MAX_FIELD_LEN+1];
    char name_state[MAX_FIELD_LEN+1];
    char code_government[MAX_FIELD_LEN+1];
    char name_government[MAX_FIELD_LEN+1];
    double latitude, longitude;

	if (fscanf(f, "%d,%d,%[^,],%d,", &comp_code, &code_suburb, name_suburb, &year) == 4) {
		s = malloc(sizeof(*s));     // allocates memory for s
		assert(s);

		s->comp_code = comp_code;
        s->code_suburb = code_suburb;
		s->name_suburb = strdup(name_suburb);     
		assert(s->name_suburb);          
		s->year = year;

        read_quoted_field(f, code_state);
        s->code_state = strdup(code_state);
        assert(s->code_state);

        read_quoted_field(f, name_state);
        s->name_state = strdup(name_state);
        assert(s->name_state);

        read_quoted_field(f, code_government);
        s->code_government = strdup(code_government);
        assert(s->code_government);

        read_quoted_field(f, name_government);
        s->name_government = strdup(name_government);
        assert(s->name_government);

        fscanf(f, "%lf,%lf", &latitude, &longitude);
        s->latitude = latitude;
        s->longitude = longitude;
        
	}
	return s;
}

// Retrieve the name of the suburb from a suburb_t struct
// Parameter changes to "void *" for polymorphic use
// Adopted from project qStud
char * get_suburb_name(void *s){
    return ((suburb_t *)s) -> name_suburb;
}

// Print detailed suburb information to output file f
// Parameter changes to "void *" for polymorphic use
void print_suburb_detail(FILE *f, void *s) {
    // Cast the generic pointer to a suburb_t pointer
    suburb_t *suburb = (suburb_t *)s;

    fprintf(f,
        "COMP20003 Code: %d, "
        "Official Code Suburb: %d, "
        "Official Name Suburb: %s, "
        "Year: %d, "
        "Official Code State: %s, "
        "Official Name State: %s, "
        "Official Code Local Government Area: %s, "
        "Official Name Local Government Area: %s, "
        "Latitude: %.7lf, "
        "Longitude: %.7lf\n",
        suburb->comp_code,
        suburb->code_suburb,
        suburb->name_suburb,
        suburb->year,
        suburb->code_state,
        suburb->name_state,
        suburb->code_government,
        suburb->name_government,
        suburb->latitude,
        suburb->longitude
    );
}

// Handle and print an qutoe enclosed string to output file f
static void print_quoted_string(FILE *f, char *str) {
    int i = 0;
    int has_comma = 0;

    // Check if the string contains a comma
    while (str[i]) {
        if (str[i] == ',') {
            has_comma = 1;
            break;
        }
        i++;
    }

    // Print the string enclosed in quotes if it contains a comma
    if (has_comma) {
        fprintf(f, "\"%s\",", str);
    } else {
        fprintf(f, "%s,", str);
    }
}

// Print surburb details in csv format to output file f
// Parameter changes to "void *" for polymorphic use
// Adopted from project qStud
void print_suburb_to_csv(FILE *f, void *s){
    suburb_t *suburb = (suburb_t *)s;
    fprintf(f, "%d,%d,%s,%d,",
    suburb->comp_code,
    suburb->code_suburb,
    suburb->name_suburb,
    suburb->year);

    // Handle strings enclosed with double quotes
    print_quoted_string(f, suburb->code_state);
    print_quoted_string(f, suburb->name_state);
    print_quoted_string(f, suburb->code_government);
    print_quoted_string(f, suburb->name_government);
    fprintf(f, "%.7lf,%.7lf\n",
    suburb->latitude,
    suburb->longitude);
}

// Free the memory allocated for the struct and its dynamically allocated fields
// Parameter changes to "void *" for polymorphic use
// Adopted from project qStud
void free_suburb(void *s) {
    free(((suburb_t *)s)->name_suburb);
	free(((suburb_t *)s)->code_state);
    free(((suburb_t *)s)->name_state);
    free(((suburb_t *)s)->code_government);
    free(((suburb_t *)s)->name_government);
	free(s);
}



