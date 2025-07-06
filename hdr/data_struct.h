#ifndef __DATA_STRUCT__H__
#define __DATA_STRUCT__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hdr/utils.h"


/* a node will represent a line in file */
typedef struct Node {
    char *line;
    struct Node *next;
} Node;

typedef Node Line;

typedef struct Macro {
    char *name;
    Line *line;
} Macro;

/* line */
Line* new_line(char *);
void delete_line_from_list(Line *);
void free_list(Line *);
Line* file_to_list(FILE *);
int list_to_file(Line *, FILE *);

/* macro */
Macro* init_macro_list(char *);
void add_line_to_macro(Macro *, Line *);
Line *copy_macro_lines(Line *);
void insert_macro_in_list(Line *, Macro *);
void free_macro(Macro *);

#endif