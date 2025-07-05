#ifndef __DATA_STRUCT__H__
#define __DATA_STRUCT__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hdr/utils.h"


/* a node will represent a line in file */
typedef struct Node {
    char *line;
    int line_number;
    struct Node *next;
} Node;

typedef Node Line;

typedef struct Macro {
    char *name;
    Line *line;
} Macro;


Line* add_new_line(char *, int);
void delete_line(Line *, int);
Line* insert_in_list(Line *, int, char *);
void free_list(Line *);
Line* file_to_list(FILE *);
int list_to_file(Line *, FILE *);

#endif