#include "../hdr/errors_handling.h"


/* -------------------------- Pre-Assembler -------------------------- */

int macro_name_valid(char *line, int line_number) {
    int id;  /* array element index */

    /* check if command appears in macro name */
    for (id = 0; id < NUM_OF_COMMANDS; id++) {
        if (strstr(line, commands[id].name) != NULL) {
            fprintf(stderr, "Error in line %d: command '%s' found in line\n", line_number, commands[id].name);
            return FALSE;
        }
    }

    /* check if instruction appears in macro name */
    for (id = 0; id < NUM_OF_INSTRUCTIONS; id++) {
        if (strstr(line, instructions[id]) != NULL) {
            fprintf(stderr, "Error in line %d: instruction '%s' found in line\n", line_number, instructions[id]);
            return FALSE;
        }
    }

    /* check if register name appears in macro name */
    for (id = 0; id < NUM_OF_REGISTERS; id++) {
        if (strstr(line, registers[id]) != NULL) {
            fprintf(stderr, "Error in line %d: register '%s' found in line\n", line_number, registers[id]);
            return FALSE;
        }
    }

    return TRUE;
}


int macro_args_num_valid(char *line, int num_of_expected_args) {
    char *line_args;
    char *line_copy;
    int num_of_args = 0;  /* number of args in line */

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_args = strtok(line_copy, WHITESPACE);

    while (line_args != NULL) {
        /* count argument */
        num_of_args++;
        /* next token/arg */
        line_args = strtok(NULL, WHITESPACE);
    }

    free(line_copy);
    return (num_of_args == num_of_expected_args);
}