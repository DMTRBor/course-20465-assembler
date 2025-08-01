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

    for (id = 0; id < NUM_OF_DIRECTIVES; id++) {
        if (strstr(line, directives[id]) != NULL) {
            fprintf(stderr,
                "Illegal argument in line %d: directive '%s' found in line\n", line_number, directives[id]);
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
    char *line_args, *line_copy;
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
        if (!isalnum(arg[id]) && arg[id] != UNDERSCORE && arg[id] != LABEL_END_SIGN) {
            fprintf(stderr,
                "Invalid label in line %d: '%s' contains invalid character '%c'\n", line_number, arg, arg[id]);
            return FALSE;
        }
    }

    return TRUE;  /* valid label */
}


int is_operands_num_valid(char *line, int num_of_operands) {
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


int is_operand_value_legal(char *operand, int line_number) {
    char *end;
    long value;
    
    /* convert string to integer */
    value = strtol(operand, &end, BASE_10);
    
    /* check if conversion was successful (no remaining characters) */
    if (*end != NULL_TERMINATOR) {
        fprintf(stderr, "Error in line %d: invalid operand value '%s'. Must be an integer\n", 
                line_number, operand);
        return FALSE;
    }
    
    /* check if value is within allowed range */
    if (value < MIN_INTEGER || value > MAX_INTEGER) {
        fprintf(stderr, "Error in line %d: operand value '%s' out of range. Must be between %d and %d\n",
                line_number, operand, MIN_INTEGER, MAX_INTEGER);
        return FALSE;
    }
    
    return TRUE;
}


int is_src_addr_method_valid(unsigned int src_operand, int op_id) {
    switch (src_operand) {
        case IMMEDIATE:
            if (!legal_addressing_methods[op_id].legal_src_addr_method.immediate_addressing)
                return FALSE;
            break;
        case DIRECT:
            if (!legal_addressing_methods[op_id].legal_src_addr_method.direct_addressing)
                return FALSE;
            break;
        case MAT:
            if (!legal_addressing_methods[op_id].legal_src_addr_method.mat_addressing)
                return FALSE;
            break;
        case REG:
            if (!legal_addressing_methods[op_id].legal_src_addr_method.reg_addressing)
                return FALSE;
            break;
    }

    return TRUE;  /* valid addressing method */
}


int is_dest_addr_method_valid(unsigned int dest_operand, int op_id) {
    switch (dest_operand) {
        case IMMEDIATE:
            if (!legal_addressing_methods[op_id].legal_dst_addr_method.immediate_addressing)
                return FALSE;
            break;
        case DIRECT:
            if (!legal_addressing_methods[op_id].legal_dst_addr_method.direct_addressing)
                return FALSE;
            break;
        case MAT:
            if (!legal_addressing_methods[op_id].legal_dst_addr_method.mat_addressing)
                return FALSE;
            break;
        case REG:
            if (!legal_addressing_methods[op_id].legal_dst_addr_method.reg_addressing)
                return FALSE;
            break;
    }

    return TRUE;  /* valid addressing method */
}


int is_legal_addressing_methods(char *op_name, unsigned int dest_operand,
                                unsigned int src_operand, int num_of_operands) {
    int id;  /* array index */
    
    /* search for operation in legal_addressing_methods array */
    for (id = 0; id < NUM_OF_OPERATIONS; id++) {
        /* find operation*/
        if (strcmp(op_name, legal_addressing_methods[id].oper->name) == STR_EQUAL) {
            /* check source and destination operand addressing methods */
            if (num_of_operands == DEST_ONLY) {
                if (!is_dest_addr_method_valid(dest_operand, id))
                    return FALSE;  /* illegal destination addressing method found */
            }
            else if (num_of_operands == SRC_AND_DEST) {
                if (!is_src_addr_method_valid(src_operand, id) ||
                    !is_dest_addr_method_valid(dest_operand, id)) {
                    return FALSE;  /* illegal addressing method found */
                }
            }

            return TRUE;  /* no operands || all addressing methods are legal */
        }
    }

    return FALSE;  /* operation name not found (though already checked) */
}


/* ---------------------------------------------------------------------- */

LineArg detect_and_validate_first_arg(char *line, int line_number) {
    char *line_args, *line_copy;
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
        line_arg_type = INSTRUCTION;
    }
    else if (is_directive(line_args)) {
        line_arg_type = DIRECTIVE;
    }
    else if (is_valid_label(line_args, line_number)) {
        line_arg_type = LABEL;
    }

    free(line_copy);
    return line_arg_type;
}