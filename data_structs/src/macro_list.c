#include "../hdr/macro.h"


Macro* init_macro_list(char *macro_name) {
    Macro *macro;
    /* allocate memory and check if allocated properly */
    if ((macro = malloc(sizeof(Macro))) == NULL) {
        fprintf(stderr, "Error in memory allocation at macro list init\n");
        return NULL;
    }

    macro->name = duplicate_str(macro_name);
    macro->line = NULL;

    return macro;
}


int add_line_to_macro(Macro *macro, Line *line) {
    Line *curr = NULL, *new_line;
    /* allocate line for copy and check if allocated properly */
    if ((new_line = malloc(sizeof(Line))) == NULL) {
        fprintf(stderr, "Error in memory allocation at adding new line to macro list\n");
        return STATUS_CODE_ERR;
    }

    /* copy line content */
    new_line->line = duplicate_str(line->line);
    new_line->next = NULL;

    /* add line to macro list */
    if (macro->line == NULL)  /* macro list empty */
        macro->line = new_line;
    else {
        /* traverse to the end and append */
        curr = macro->line;

        while (curr->next != NULL) {
            curr = curr->next;  /* go to next line */
        }

        curr->next = new_line;
    }
	
	return STATUS_CODE_OK;
}


Line *copy_macro_lines(Line *origin_lines) {  /* origin_lines - original macro list */
    Line *copy_head = NULL, *copy_tail = NULL, *new_line = NULL;

    if (origin_lines == NULL)  /* invalid pointer */
        return NULL;

    while (origin_lines != NULL) {
        /* allocate memory and check if allocated properly */
        if ((new_line = malloc(sizeof(Line))) == NULL) {
            fprintf(stderr,
                    "Error in memory allocation for new line, while copying macro lines\n");
            return NULL;
        }

        new_line->line = duplicate_str(origin_lines->line);
        new_line->next = NULL;

        if (copy_head == NULL)  /* amcro copy list is empty */
            copy_head = copy_tail = new_line;
        else {
            copy_tail->next = new_line;  /* add at end of the list the next line */
            copy_tail = new_line;
        }

        origin_lines = origin_lines->next;  /* go to next line */
    }

    return copy_head;
}


int insert_macro_in_list(Line *line, Macro *macro) {
    Line *macro_copy = NULL, *macro_tail = NULL;
    /* check if line and macro are valid */
    if (line == NULL || macro == NULL || macro->line == NULL)
        return STATUS_CODE_ERR;

    /* use copy of macro for reusal in repeated calls */
    if ((macro_copy = copy_macro_lines(macro->line)) == NULL)  /* validate copying */
        return STATUS_CODE_ERR;

    macro_tail = macro_copy;

    /* find last line in macro */
    while (macro_tail->next)
        macro_tail = macro_tail->next;

    /* replace macro call line in
       list with first macro line */
    free(line->line);
    line->line = duplicate_str(macro_copy->line);

    /* insert the rest of the macro
       lines after target */
    macro_tail->next = line->next;
    line->next = macro_copy->next;

    /* macro first line already copied
       instead of macro call */
    free(macro_copy->line);
    free(macro_copy);

	return STATUS_CODE_OK;
}


void free_macro(Macro *macro) {
	Line *curr, *temp = NULL;
    /* check if macro pointer is valid */
	if (macro == NULL)
        return;

    curr = macro->line;

    while (curr != NULL) {  /* free macro lines list */
        temp = curr;
        curr = curr->next;
        free(temp->line);
        free(temp);
    }

    free(macro->name);  /* free macro node and name */
    free(macro);
}
