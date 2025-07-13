#include "../hdr/data_struct.h"


/* -------------------------- Lines -------------------------- */

Line* new_line(char *line) {
    Line *new_line = malloc(sizeof(Line));
    /* check if allocated properly */
    if (new_line == NULL)
        return NULL;

    new_line->line = strdup(line);  /* copy line */
    new_line->next = NULL;
    return new_line;
}


void delete_line_from_list(Line *curr) {
    /* last line will not be deleted */
    if (curr == NULL || curr->next == NULL)
        return;

    Line *temp = curr->next;

    /* copy next line's data into current ptr */
    free(curr->line);
    curr->line = strdup(temp->line);
    curr->next = temp->next;

    /* free redundant next line */
    free(temp->line);
    free(temp);
}


void free_list(Line *curr) {
    while (curr != NULL) {
        Line *temp = curr;
        curr = curr->next;
        free(temp->line);
        free(temp);
    }
}


Line* file_to_list(FILE *fp) {
    Line *start = NULL, *end = NULL;
    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line_too_long(line, fp)) {
            fprintf(stderr, "Line too long in file, max length is %d characters\n", MAX_LINE_LEN - 1);
            return NULL;
        }
        /* cut ending new line */
        line[strcspn(line, NEWLINE_STR)] = NULL_TERMINATOR;
        Line *new = new_line(line);

        if (start == NULL)
            start = end = new;
        else {
            end->next = new;
            end = new;
        }
    }

    return start;
}


int list_to_file(Line *curr, FILE *fp) {
    int line_number = 1;  /* initialize line number */
    
    if (fp == NULL)
        return STATUS_CODE_ERR;

    while (curr != NULL) {
        /* write line to file stream */
        /* negative value indicates writing failed */
        if (fprintf(fp, "%s\n", curr->line) < 0) {
            fprintf(stderr, "Error writing to file at line %d\n", line_number);
            return STATUS_CODE_ERR;
        }
        curr = curr->next;
        line_number++;
    }

    return STATUS_CODE_OK;
}

/* -------------------------- Macro -------------------------- */

Macro* init_macro_list(char *mcro_name) {
    Macro *mcro = malloc(sizeof(Macro));
    /* check if allocated properly */
    if (mcro == NULL)
        return NULL;

    mcro->name = strdup(mcro_name);
    mcro->line = NULL;

    return mcro;
}


void add_line_to_macro(Macro *mcro, Line *line) {
    /* allocate line for copy */
    Line *new_line = malloc(sizeof(Line));
    /* check if allocated properly */
    if (new_line == NULL)
        return;

    /* copy line content */
    new_line->line = strdup(line->line);
    new_line->next = NULL;

    /* add line to macro list */
    if (mcro->line == NULL)
        mcro->line = new_line;
    else {
        /* traverse to the end and append */
        Line *curr = mcro->line;

        while (curr->next != NULL) {
            curr = curr->next;
        }

        curr->next = new_line;
    }
}


Line *copy_macro_lines(Line *origin_lines) {
    if (origin_lines == NULL)
        return NULL;

    Line *copy_head = NULL, *copy_tail = NULL;

    while (origin_lines != NULL) {
        Line *new_line = malloc(sizeof(Line));
        /* check if allocated properly */
        if (new_line == NULL)
            return NULL;

        new_line->line = strdup(origin_lines->line);
        new_line->next = NULL;

        if (copy_head == NULL)
            copy_head = copy_tail = new_line;
        else {
            copy_tail->next = new_line;
            copy_tail = new_line;
        }

        origin_lines = origin_lines->next;
    }

    return copy_head;
}


void insert_macro_in_list(Line *line, Macro *macro) {
    if (line == NULL || macro == NULL || macro->line == NULL)
        return;

    /* use copy of macro for reusal in repeated calls */
    Line *macro_copy = copy_macro_lines(macro->line);
    /* validate copying */
    if (macro_copy == NULL)
        return;

    Line *macro_tail = macro_copy;

    /* find last line in macro */
    while (macro_tail->next)
        macro_tail = macro_tail->next;

    /* replace macro label line in
       list with first macro line */
    free(line->line);
    line->line = strdup(macro_copy->line);

    /* insert the rest of the macro
       lines after target */
    macro_tail->next = line->next;
    line->next = macro_copy->next;

    /* macro first line already copied
       instead of label */
    free(macro_copy->line);
    free(macro_copy);
}


void free_macro(Macro *mcro) {
    Line *curr = mcro->line;

    while (curr != NULL) {
        Line *temp = curr;
        curr = curr->next;
        free(temp->line);
        free(temp);
    }

    free(mcro->name);
    free(mcro);
}


/* -------------------------- Macro + Lines -------------------------- */
void free_list_and_macro(Line *first_line, Macro *macro_curr_line) {
    /* free lines list */
    free_list(first_line);

    /* free macro list if exists */
    if (macro_curr_line != NULL)
        free_macro(macro_curr_line);
}