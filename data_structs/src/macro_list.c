#include "../hdr/macro.h"


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
    Line *curr = NULL;
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
        curr = mcro->line;

        while (curr->next != NULL) {
            curr = curr->next;
        }

        curr->next = new_line;
    }
}


Line *copy_macro_lines(Line *origin_lines) {
    Line *copy_head = NULL, *copy_tail = NULL, *new_line = NULL;

    if (origin_lines == NULL)
        return NULL;

    while (origin_lines != NULL) {
        new_line = malloc(sizeof(Line));
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
    Line *macro_copy = NULL, *macro_tail = NULL;
    /* check if line and macro are valid */
    if (line == NULL || macro == NULL || macro->line == NULL)
        return;

    /* use copy of macro for reusal in repeated calls */
    macro_copy = copy_macro_lines(macro->line);
    /* validate copying */
    if (macro_copy == NULL)
        return;

    macro_tail = macro_copy;

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
    Line *temp = NULL;

    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free(temp->line);
        free(temp);
    }

    free(mcro->name);
    free(mcro);
}