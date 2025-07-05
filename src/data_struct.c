#include "../hdr/data_struct.h"


/* -------------------------- Lines -------------------------- */

Line* add_new_line(char *line, int line_number) {
    Line *new_line = malloc(sizeof(Line));
    /* check if allocated properly */
    if (new_line == NULL)
        return NULL;

    new_line->line = strdup(line);  /* copy line */
    new_line->line_number = line_number;
    new_line->next = NULL;
    return new_line;
}


void delete_line(Line *start, int line_number) {
    Line *curr = start, *prev = NULL;

    while (curr != NULL) {
        if (curr->line_number == line_number) {
            if (prev != NULL)
                prev->next = curr->next;
            else
                start = curr->next;

            free(curr->line);
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    return start;
}


Line* insert_in_list(Line *start, int start_line, char *line_content) {
    Line *curr = start;

    while (curr != NULL) {
        if (curr->line_number == start_line) {
            Line *new_line = add_new_line(line_content, start_line + 1);
            new_line->next = curr->next;
            curr->next = new_line;
            break;
        }
        curr = curr->next;
    }

    return start;
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
    int line_number = 1;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line_too_long(line, fp))
            return NULL;

        line[strcspn(line, NEWLINE_STR)] = NULL_TERMINATOR;
        Line *new_line = add_new_line(line, line_number++);

        if (start == NULL)
            start = end = new_line;
        else {
            end->next = new_line;
            end = new_line;
        }
    }

    return start;
}


int list_to_file(Line *start, FILE *fp) {
    if (fp == NULL)
        return 0;

    while (start != NULL) {
        /* write line to file stream */
        /* negative value indicates writing failed */
        if (fprintf(fp, "%s\n", start->line) < 0) {
            fprintf(stderr, "Error writing to file at line %d\n", start->line_number);
            return STATUS_CODE_ERR;
        }
        start = start->next;
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
    if (mcro->line == NULL)
        mcro->line = line;
    else {
        Line *curr = mcro->line;

        while (curr->next)
            curr = curr->next;

        curr->next = line;
    }
}


void free_macro(Macro *m) {
    Line *curr = m->line;

    while (curr != NULL) {
        Line *tmp = curr;
        curr = curr->next;
        free(tmp->line);
        free(tmp);
    }

    free(m->name);
    free(m);
}