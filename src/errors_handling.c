#include "../hdr/errors_handling.h"


/* -------------------------- Pre-Assembler -------------------------- */

int is_macro_name_valid(char *line, int line_number) {
    int id;  /* array element index */

    /* check if operation appears in macro name */
    for (id = 0; id < NUM_OF_OPERATIONS; id++) {
        if (strstr(line, operations[id].name) != NULL) {
            fprintf(stderr,
                "Illegal argument in line %d: operation '%s' found in line\n", line_number, operations[id].name);
            return FALSE;
        }
    }

    /* check if instruction appears in macro name */
    for (id = 0; id < NUM_OF_INSTRUCTIONS; id++) {
        if (strstr(line, instructions[id]) != NULL) {
            fprintf(stderr,
                "Illegal argument in line %d: instruction '%s' found in line\n", line_number, instructions[id]);
            return FALSE;
        }
    }

    /* check if register name appears in macro name */
    for (id = 0; id < NUM_OF_REGISTERS; id++) {
        if (strstr(line, registers[id]) != NULL) {
            fprintf(stderr,
                "Illegal argument in line %d: register '%s' found in line\n", line_number, registers[id]);
            return FALSE;
        }
    }

    return TRUE;
}


int is_macro_args_num_valid(char *line, int num_of_expected_args) {
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


/* -------------------------- First / Second Pass -------------------------- */

int is_operation(char *arg) {
    int id;  /* array index */

    /* check if argument is an operation */
    for (id = 0; id < NUM_OF_OPERATIONS; id++) {
        if (strcmp(arg, operations[id].name) == STR_EQUAL) {
            return TRUE;
        }
    }

    return FALSE;  /* not an operation */
}


int is_instruction(char *arg) {
    int id;  /* array index */

    /* check if argument is an instruction */
    for (id = 0; id < NUM_OF_INSTRUCTIONS; id++) {
        if (strcmp(arg, instructions[id]) == STR_EQUAL) {
            return TRUE;
        }
    }

    return FALSE;  /* not an instruction */
}


int is_valid_label(char *arg, int line_number) {
    int id;  /* array index */

    /* check if label starts with a letter */
    if (!isalpha(arg[0])) {
        fprintf(stderr,
            "Invalid label in line %d: '%s' must start with a letter\n", line_number, arg);
        return FALSE;
    }

    /* check if label contains only valid characters */
    for (id = 0; arg[id] != NULL_TERMINATOR; id++) {
        if (!isalnum(arg[id]) && arg[id] != UNDERSCORE) {
            fprintf(stderr,
                "Invalid label in line %d: '%s' contains invalid character '%c'\n", line_number, arg, arg[id]);
            return FALSE;
        }
    }

    return TRUE;  /* valid label */
}


/**
 * This function receives a line
 */
LineArg detect_and_validate_first_arg(char *line, int line_number) {
    char *line_args;
    char *line_copy;
    LineArg line_arg_type = ERROR;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_args = strtok(line_copy, WHITESPACE);

    if (*line == NULL_TERMINATOR || strcmp(line_args, NEWLINE_STR) == STR_EQUAL) {
        line_arg_type = EMPTY_LINE;
    }
    else if (line_args[0] == COMMENT_SIGN) {
        line_arg_type = COMMENT;
    }
    else if (is_operation(line_args)) {
        line_arg_type = OPERATION;
    }
    else if (is_instruction(line_args)) {
        line_arg_type = INSTRUCTION;
    }
    else if (is_valid_label(line_args, line_number)) {
        line_arg_type = LABEL;
    }

    free(line_copy);
    return line_arg_type;
}


int is_operands_num_valid(char *line, int num_of_operands) {
    char *line_args;
    char *line_copy;
    char *operation_name;
    int id;

    /* copy line for processing */
    line_copy = strdup(line);
    /* get first token (operation name) */
    operation_name = strtok(line_copy, WHITESPACE);

    /* search for operation and compare with expected_num_of_operands table */
    for (id = 0; id < NUM_OF_OPERATIONS; id++) {
        if (strcmp(operation_name, expected_num_of_operands[id].oper->name) == STR_EQUAL) {
            /* found operation, compare operand count */
            free(line_copy);
            return (num_of_operands == expected_num_of_operands[id].expected_num_of_operands);
        }
    }

    /* operation not found */
    free(line_copy);
    return FALSE;
}


int is_operands_legal(char *line, int num_of_operands) {
    char *line_args;
    char *line_copy;
    char *operation_name;
    int id;

    /* copy line for processing */
    line_copy = strdup(line);
    /* get first token (operation name) */
    operation_name = strtok(line_copy, WHITESPACE);

    /* TODO */

    /* operation not found */
    free(line_copy);
    return FALSE;
}


/**
 * This function receives a line and line number, parses line arguments 
 * with different delimiters and returns the total number of arguments.
 */
int get_num_of_operands(char *line, int line_number) {
    char *line_args;
    char *line_copy;
    int num_of_args = 0;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with different delimiters */
    line_args = strtok(line_copy, OP_DELIMITERS);

    while (line_args != NULL) {
        num_of_args++;
        
        /* get next token/arg */
        line_args = strtok(NULL, OP_DELIMITERS);
    }

    /* discount operation name from count */
    num_of_args--;

    /* validate number of operands */
    if (is_operands_num_valid(line, num_of_args)) {
        fprintf(stderr, "Error in line %d: invalid number of operands\n", line_number);
        num_of_args = OPERANDS_NUMBER_ERROR;
    }

    free(line_copy);
    return num_of_args;
}