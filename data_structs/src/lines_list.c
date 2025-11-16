#include "../hdr/line.h"


Line* new_line(char *line) {
    Line *new_line;
    /* allocate memory and check if allocated properly */
    if ((new_line = malloc(sizeof(Line))) == NULL) {
        fprintf(stderr, "Error in memory allocation for new line\n");
        return NULL;
    }

    new_line->line = duplicate_str(line);  /* copy line */
    new_line->next = NULL;
    return new_line;
}


void delete_line_from_list(Line *curr_line) {
    Line *temp = NULL;
    /* check if pointer is valid, last line will not be deleted */
    if (curr_line == NULL || curr_line->next == NULL)
        return;

    temp = curr_line->next;  /* save next line */

    /* copy next line's data into current ptr */
    free(curr_line->line);
    curr_line->line = duplicate_str(temp->line);
    curr_line->next = temp->next;

    /* free redundant next line */
    free(temp->line);
    free(temp);
}


void free_lines_list(Line *list) {
    Line *temp = NULL;

    while (list != NULL) {
        temp = list;  /* store current line */
        list = list->next;  /* go to next line */
        free(temp->line);  /* delete line */
        free(temp);
    }
}


Line* file_to_lines_list(FILE *fp) {
    Line *start = NULL, *end = NULL, *new = NULL;
    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), fp) != NULL) {
        /* count line characters until newline (if exists) and check if */
        /* newline not found at the end of the line and not reached EOF */
        if (line[strcspn(line, NEWLINE_STR)] != NEWLINE_CHAR && !feof(fp)) {
            fprintf(stderr, "Line too long in file, max length is %d characters\n", MAX_LINE_LEN - 1);
            return NULL;
        }
        /* cut ending new line */
        line[strcspn(line, NEWLINE_STR)] = NULL_TERMINATOR;
        /* allocate memory and check if allocated */
        if ((new = new_line(line)) == NULL) {
            fprintf(stderr,
                    "Error in memory allocation for new line in creating list of lines\n");
            return NULL;
        }

        if (start == NULL)  /* lines list is empty */
            start = end = new;
        else {
            end->next = new;  /* add next line on list end */
            end = new;
        }
    }

    return start;  /* pointer to first line */
}


int lines_list_to_file(Line *list, FILE *fp) {
    int line_number = 1;  /* initialize line number */
    
    if (fp == NULL)  /* file pointer is invalid */
        return STATUS_CODE_ERR;

    while (list != NULL) {
        /* write line to file stream */
        /* negative value indicates writing failed */
        if (fprintf(fp, "%s\n", list->line) < MIN_CHARS_WRITTEN) {
            fprintf(stderr, "Error writing to file at line %d\n", line_number);
            return STATUS_CODE_ERR;
        }
        list = list->next;  /* go to next line */
        line_number++;
    }

    return STATUS_CODE_OK;
}
