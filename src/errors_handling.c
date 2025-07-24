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
    free(line_copy);
    return TRUE;

    /* operation not found */
    free(line_copy);
    return FALSE;
}


/* ---------------------------------------------------------------------- */

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
    if (!is_operands_num_valid(line, num_of_args)) {
        fprintf(stderr, "Error in line %d: invalid number of operands\n", line_number);
        num_of_args = OPERANDS_NUM_ERROR;
    }

    free(line_copy);
    return num_of_args;
}


/* --------------------------------- Encoding --------------------------------- */

int encode_operation(MemoryUnit **table,
                     char *op_name,
                     unsigned int dest_operand,
                     unsigned int src_operand,
                     int line_number) {
    int id;  /* array index */
    unsigned int encoding_type, opcode;
    MemoryUnit *new;

    /* search for operation in operations table */
    for (id = 0; id < NUM_OF_OPERATIONS; id++) {
        if (strcmp(op_name, operations[id].name) == STR_EQUAL) {
            /* allocate new memory unit */
            new = new_mem_unit();

            if (new == NULL) {
                fprintf(stderr, "Memory allocation error in line %d\n", line_number);
                return STATUS_CODE_ERR;  /* memory allocation failed */
            }

            /* found operation, encode it */
            encoding_type = A;
            opcode = operations[id].code;
            /* set fields in memory unit */
            set_word_fields(new,
                            encoding_type,
                            dest_operand,
                            src_operand,
                            opcode);
            /* add to table */
            add_mem_unit_to_table(table, new);
            
            return STATUS_CODE_OK;
        }
    }

    fprintf(stderr, "Error in line %d: unknown operation '%s'\n", line_number, op_name);
    return STATUS_CODE_ERR;  /* unknown operation */
}


int encode_op_sentence(char *op_line,
                       int num_of_operands,
                       MemoryUnit **table,
                       int line_number) {
    char *line_args, *line_copy, *op_name;
    LineArg operand_type;

    /* word fields -initialize with zeros */
    unsigned int encoding_type = A;
    unsigned int dest_operand = IMMEDIATE;
    unsigned int src_operand = IMMEDIATE;
    unsigned int opcode = operations[0].code;

    /* words and args counter */
    int L = 0, arg_id = 0;

    /* copy line for processing */
    line_copy = strdup(op_line);
    /* tokenize with different delimiters */
    line_args = strtok(line_copy, OP_DELIMITERS);
    
    /* extract info for operation encoding */
    while (line_args != NULL) {
        if (arg_id == FIRST_ARG)  /* get operation name */
            op_name = strdup(line_args);
        else {
            if ((operand_type = get_operand_type(line_args)) == ERROR) {
                fprintf(stderr, "Error in line %d: invalid operand type - %s\n", line_number, line_args);
                free(line_copy);
                free(op_name);
                return WORDS_NUM_ERROR;  /* invalid operand type */
            }

            /* set operand field based on type */
            set_word_operand_field(operand_type, arg_id, num_of_operands,
                                   &dest_operand, &src_operand);
        }

        /* get next token/arg */
        line_args = strtok(NULL, OP_DELIMITERS);
        arg_id++;
    }

    /* encode operation */
    if (encode_operation(table, op_name, dest_operand, src_operand, line_number) == STATUS_CODE_ERR) {
        free(line_copy);
        free(op_name);
        return WORDS_NUM_ERROR;  /* encoding failed */
    }
    /* count encoded operation */
    L++;

    /* encode operands */


    /* check if number of words is not exceeded */
    if (L > MAX_WORDS_IN_SENTENCE) {
        fprintf(stderr, "Error in line %d: too many words in sentence\n", line_number);
        free(line_copy);
        free(op_name);
        return WORDS_NUM_ERROR;
    }

    free(line_copy);
    free(op_name);
    return L;
}