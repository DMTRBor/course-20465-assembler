#ifndef __LINE_H__
#define __LINE_H__

#include "../../utils/hdr/utils.h"


/* a line from file in
   list of lines */
typedef struct Line {
    char *line;  /* line content */
    struct Line *next;  /* pointer to next line in list */
} Line;


/**
 * This function receives a string
 * that holds the content from a line
 * read from a file and it creates a
 * new line node, intialized with
 * default values. The function returns
 * an address of the line node that was
 * created.
 */
Line* new_line(char *);


/**
 * The function receives address of the
 * line node in a list of lines and
 * deletes the line from list. The line
 * (node) previous to current line will
 * point after the function called, to
 * the line in list that placed after
 * the current line.
 * The function does not return any value.
 */
void delete_line_from_list(Line *);


/**
 * The function receives address of the head of
 * lines list, and frees the list by
 * deallocating all the memory allocated for the
 * lines list (linked list) including the fields
 * of each line, if needed.
 * The function does not return any value.
 */
void free_lines_list(Line *);


/**
 * The function receives a pointer
 * to a stream read from file and
 * creates a linked list of lines,
 * where each line will be
 * represented by a node in list.
 * The function returns a pointer
 * to the first line in the list
 * of lines created.
 */
Line* file_to_lines_list(FILE *);


/**
 * The function receives address of the
 * first line in list of lines and
 * a pointer to a beginning of a file
 * opened in writing mode and writes
 * each line content from the list of
 * lines to a new line in file. If any
 * error occurs during writing to a file
 * the function will return an error status
 * STATUS_CODE_ERR. Otherwise, STATUS_CODE_OK
 * will be returned, indicating write success.
 */
int lines_list_to_file(Line *, FILE *);

#endif