#ifndef __MACRO_H__
#define __MACRO_H__

#include "line.h"
#include "../../utils/hdr/utils.h"


/* macro list unit */
typedef struct Macro {
    char *name;  /* macro name */
    Line *line;  /* macro line content */
} Macro;


/**
 * The function receives a string
 * with macro name and returns a
 * pointer to a start of a created
 * list of lines that belong to a
 * specific macro whose name is given.
 * The field "line" points to a start
 * of lines list and utilized a Line
 * structure (node).
 */
Macro* init_macro_list(char *);


/**
 * The function receives:
 * - pointer to a list of macro lines
 * - pointer to line node
 * It adds this line to list of macro
 * lines, at the end.
 * The function returns STATUS_CODE_OK
 * if the insertion was successful and
 * STATUS_CODE_ERR otherwise.
 */
int add_line_to_macro(Macro *, Line *);


/**
 * The function receives a pointer to a
 * list of lines, that belong to some
 * macro and copies the list of lines.
 * The function returns a pointer to the
 * start of the copied list of macro lines.
 */
Line *copy_macro_lines(Line *);


/**
 * The function receives:
 * - pointer to a list of total lines in file
 * - pointer to a macro list
 * It inserts the macro list lines in list of
 * total file lines, by searching for macro
 * calls in lines list and replacing the call
 * with macro lines.
 * The function returns STATUS_CODE_OK
 * if the replace was successful and
 * STATUS_CODE_ERR otherwise.
 */
int insert_macro_in_list(Line *, Macro *);


/**
 * The function receives a pointer to a head of
 * macro lines list, and frees the list by
 * deallocating all the memory allocated for the
 * lines list (linked list) including the fields
 * of each line, if needed, and the macro name.
 * The function does not return any value.
 */
void free_macro(Macro *);

#endif
