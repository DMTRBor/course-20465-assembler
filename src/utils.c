#include "../hdr/utils.h"


/**
 * This function receives a decimal number string
 * representation and an empty string, converts the
 * decimal number to base 4 representation
 * and places the converted string representation of
 * the number in base 4 in an empty string.
 */
void decimal_to_base4(char *decimal, char *converted) {
    /* temporary converted number storage */
    char temp[sizeof(converted) / sizeof(char)];
    int decimal_num = 0;
    int power = 2;  /* hundreds */
    int i = 0, j;

    /* skip first zero, address starts from 100 */
    decimal++;
    
    /* convert string to integer */
    while (*decimal != NULL_TERMINATOR) {
        decimal_num += (*decimal - ZERO) * pow(BASE_10, power);
        decimal++;
        power--;
    }
    
    /* store reversed conversion of decimal to base 4 */
    while (decimal_num > 0) {
        temp[i++] = (decimal_num % BASE_4) + ZERO;
        decimal_num /= BASE_4;
    }
    temp[i] = NULL_TERMINATOR;
    
    /* revert the number in base 4, skip null terminator */
    for (j = i - 1; j >= 0; j--) {
        *converted = temp[j];
        converted++;
    }
    *converted = NULL_TERMINATOR;
}


/**
 * This function receives a data
 * stream read from a file and
 * returns the size of the stream/
 * content in bytes.
 */
long file_content_size(FILE* fp) {
    long content_size;

    fseek(fp, 0, SEEK_END);
    content_size = ftell(fp);
    rewind(fp);  /* return to start of the file */

    return content_size;
}


/**
 * This function receives a string
 * that represents a filename, opens
 * the file for reading and returns a
 * pointer to file if succeeded in
 * reading the content. If failed to
 * read from file (not exitent, permission),
 * the function returns null.
 */
FILE* open_file(char *filename, char *permission) {
    FILE *fp = fopen(filename, permission);

    if (fp == NULL) {
        fprintf(stderr, "Can't open file - %s\n", filename);
        fclose(fp);
        return NULL;
    }

    return fp;
}


/**
 * This function receives a line
 */
LineArg detect_arg_type(char *line) {
    char *line_args, *line_copy;
    LineArg line_arg_type = ERROR;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_args = strtok(line_copy, WHITESPACE);

    while (line_args != NULL) {
        if (strcmp(line_args, NEWLINE_STR) == STR_EQUAL) {
            line_arg_type = EMPTY_LINE;
            break;
        }
        else if (line_args[0] == COMMENT_SIGN) {
            line_arg_type = COMMENT;
            break;
        }
        else if (strcmp(line_args, MACRO_START) == STR_EQUAL) {
            line_arg_type = MCRO;
            break;
        }
        else if (strcmp(line_args, MACRO_END) == STR_EQUAL) {
            line_arg_type = MCROEND;
            break;
        }
        /* next token/arg */
        line_args = strtok(NULL, WHITESPACE);
    }

    free(line_copy);
    return line_arg_type;
}


/* -------------------------- Pre-Assembler -------------------------- */

char* get_macro_name(char *line) {
    char *line_args;
    int arg_id = 0;  /* token index */

    /* tokenize with whitespaces */
    line_args = strtok(line, WHITESPACE);

    while (line_args != NULL) {
        /* macro name goes after mcro label */
        if (arg_id == 1)
            break;

        /* next token/arg */
        line_args = strtok(NULL, WHITESPACE);
        arg_id++;
    }

    return line_args;
}


int is_macro_call(char *line, char *macro_name) {
    char *line_args, *line_copy;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_args = strtok(line_copy, WHITESPACE);

    while (line_args != NULL) {
        /* macro called */
        if (strcmp(line_args, macro_name) == STR_EQUAL) {
            free(line_copy);
            return TRUE;
        }

        /* next token/arg */
        line_args = strtok(NULL, WHITESPACE);
    }

    free(line_copy);
    return FALSE;
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


int is_register(char *arg) {
    int id;  /* array index */

    /* check if argument is a register */
    for (id = 0; id < NUM_OF_REGISTERS; id++) {
        if (strcmp(arg, registers[id]) == STR_EQUAL)
            return TRUE;
    }

    return FALSE;  /* not a register */
}


LineArg get_operand_type(char *operand) {
    LineArg operand_type = ERROR;
    int id;
    
    /* check for immediate addressing */
    if (strchr(operand, IMMEDIATE_ADDR_SIGN) != NULL)
        operand_type = IMMEDIATE_ADDR;

    /* check for matrix addressing */
    else if (strchr(operand, MAT_LEFT_BRACE) != NULL && strchr(operand, MAT_RIGHT_BRACE) != NULL)
        operand_type = MATRIX_ADDR;

    /* check if it's a register addressing */
    else {
        if (is_register(operand))
            operand_type = REGISTER_ADDR;
        else {
            /* check if direct addressing (label) */
            for (id = 0; operand[id] != NULL_TERMINATOR; id++) {
                if (isalnum(operand[id])) {  /* possibly a label */
                    operand_type = DIRECT_ADDR;
                    break;
                }
            }
        }
    }

    return operand_type;
}


AddrMethodCode get_operand_code_from_type(LineArg operand_type) {
    if (operand_type == IMMEDIATE_ADDR)
        return IMMEDIATE;
    else if (operand_type == DIRECT_ADDR)
        return DIRECT;
    else if (operand_type == MATRIX_ADDR)
        return MAT;
    else if (operand_type == REGISTER_ADDR)
        return REG;
}


void set_word_operand_field(LineArg operand_type,
                            int arg_id,
                            int num_of_operands, 
                            unsigned int *dest_operand,
                            unsigned int *src_operand) {
    if (num_of_operands == DEST_ONLY)
        *dest_operand = get_operand_code_from_type(operand_type);
    else if (num_of_operands == SRC_AND_DEST) {
        if (arg_id == FIRST_ARG_ID)
            *src_operand = get_operand_code_from_type(operand_type);
        else if (arg_id == SEC_ARG_ID)
            *dest_operand = get_operand_code_from_type(operand_type);
    }
}