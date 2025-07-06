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

    /* free redundant next node */
    free(temp->line);
    free(temp);
}


void insert_line_in_list(Line *prev, Line *new) {
    if (prev == NULL || new == NULL)
        return;

    new->next = prev->next;
    prev->next = new;
}


void free_list(Line *start) {
    while (start != NULL) {
        Line *temp = start;
        start = start->next;
        free(temp->line);
        free(temp);
    }
}


Line* file_to_list(FILE *fp) {
    Line *start = NULL, *end = NULL;
    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line_too_long(line, fp))
            return NULL;

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


int list_to_file(Line *start, FILE *fp) {
    int line_number = 1;  /* initialize line number */
    
    if (fp == NULL)
        return STATUS_CODE_ERR;

    while (start != NULL) {
        /* write line to file stream */
        /* negative value indicates writing failed */
        if (fprintf(fp, "%s\n", start->line) < 0) {
            fprintf(stderr, "Error writing to file at line %d\n", line_number);
            return STATUS_CODE_ERR;
        }
        start = start->next;
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


void free_macro(Macro *mcro) {
    Line *curr = mcro->line;

    while (curr != NULL) {
        Line *tmp = curr;
        curr = curr->next;
        free(tmp->line);
        free(tmp);
    }

    free(mcro->name);
    free(mcro);
}