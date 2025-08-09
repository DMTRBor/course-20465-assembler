#ifndef __MACRO_H__
#define __MACRO_H__

#include "line.h"
#include "../../utils/hdr/utils.h"


/* macro list */
typedef struct Macro {
    char *name;
    Line *line;
} Macro;


/* forward declarations */
Macro* init_macro_list(char *);
void add_line_to_macro(Macro *, Line *);
Line *copy_macro_lines(Line *);
void insert_macro_in_list(Line *, Macro *);
void free_macro(Macro *);

#endif