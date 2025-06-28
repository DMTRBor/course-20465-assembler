#ifndef __DATA_STRUCT__H__
#define __DATA_STRUCT__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int inside_macro = 0;
    char *line;
    int line_number;
    struct Node *next;
} Node;

#endif