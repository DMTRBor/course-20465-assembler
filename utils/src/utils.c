#include "../hdr/utils.h"


void decimal_to_base4(char *decimal, char *converted) {
    /* temporary converted number storage */
    char temp[MAX_DIGITS_BASE_4 + 1];  /* +1 for null terminator */
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


long file_content_size(FILE* fp) {
    long content_size;

    fseek(fp, 0, SEEK_END);
    content_size = ftell(fp);
    rewind(fp);  /* return to start of the file */

    return content_size;
}


FILE* open_file(char *filename, char *permission) {
    FILE *fp = fopen(filename, permission);

    if (fp == NULL) {
        fprintf(stderr, "Can't open file - %s\n", filename);
        fclose(fp);
        return NULL;
    }

    return fp;
}


LineArg detect_arg_type(char *line) {
    char *line_arg, *line_copy;
    LineArg line_arg_type = ERROR;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_arg = strtok(line_copy, WHITESPACE);

    while (line_arg != NULL) {
        if (strcmp(line_arg, NEWLINE_STR) == STR_EQUAL) {
            line_arg_type = EMPTY_LINE;
            break;
        }
        else if (line_arg[0] == COMMENT_SIGN) {
            line_arg_type = COMMENT;
            break;
        }
        else if (strcmp(line_arg, MACRO_START) == STR_EQUAL) {
            line_arg_type = MCRO;
            break;
        }
        else if (strcmp(line_arg, MACRO_END) == STR_EQUAL) {
            line_arg_type = MCROEND;
            break;
        }
        /* next token/arg */
        line_arg = strtok(NULL, WHITESPACE);
    }

    free(line_copy);
    return line_arg_type;
}


/* -------------------------- Pre-Assembler -------------------------- */

char* get_macro_name(char *line) {
    char *line_arg;
    int arg_id = 0;  /* token index */

    /* tokenize with whitespaces */
    line_arg = strtok(line, WHITESPACE);

    while (line_arg != NULL) {
        /* macro name goes after mcro label */
        if (arg_id == 1)
            break;

        /* next token/arg */
        line_arg = strtok(NULL, WHITESPACE);
        arg_id++;
    }

    return line_arg;
}


int is_macro_call(char *line, char *macro_name) {
    char *line_arg, *line_copy;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_arg = strtok(line_copy, WHITESPACE);

    while (line_arg != NULL) {
        /* macro called */
        if (strcmp(line_arg, macro_name) == STR_EQUAL) {
            free(line_copy);
            return TRUE;
        }

        /* next token/arg */
        line_arg = strtok(NULL, WHITESPACE);
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


int is_directive(char *arg) {
    int id;  /* array index */

    /* check if argument is a directive */
    for (id = 0; id < NUM_OF_DIRECTIVES; id++) {
        if (strcmp(arg, directives[id]) == STR_EQUAL) {
            return TRUE;
        }
    }

    return FALSE;  /* not a directive */
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


int is_expected_directive(char *direc, char *expected) {
    char *line_arg, *line_copy;
    /* words and args counter */
    int is_expected = FALSE;

    /* copy line for processing */
    line_copy = strdup(direc);
    /* tokenize with different delimiters */
    line_arg = strtok(line_copy, OP_DELIMITERS);

    if (strcmp(line_arg, expected) == STR_EQUAL)
        is_expected = TRUE;

    free(line_copy);
    return is_expected;
}


int get_num_of_arguments(char *line) {
    char *line_arg, *line_copy;
    int num_of_args = 0;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with different delimiters */
    line_arg = strtok(line_copy, OP_DELIMITERS);

    while (line_arg != NULL) {
        num_of_args++;
        
        /* get next token/arg */
        line_arg = strtok(NULL, OP_DELIMITERS);
    }

    /* discount operation/directive name from count */
    num_of_args--;

    free(line_copy);
    return num_of_args;
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
    
    return IMMEDIATE;  /* default operand code */
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


char* get_label_name(char **line_with_label) {
    /* find label end sign */
    char *lbl_end = strchr(*line_with_label, LABEL_END_SIGN);
    char *label_name;
    size_t len;

    /* get label length */
    len = lbl_end - *line_with_label;
    if ((label_name = (char *)malloc(len)) == NULL)
        return NULL; /* allocation failed */

    strncpy(label_name, *line_with_label, len);
    label_name[len] = NULL_TERMINATOR;

    /* set pointer after label end sign, skip whitespaces */
    lbl_end++;
    while (*lbl_end && isspace((unsigned char)*lbl_end)) {
        lbl_end++;
    }

    /* point to what goes after the label */
    *line_with_label = lbl_end;
    return label_name;
}


int to_int(char *num_str, int *num_int, int line_number) {
    char *end;
    long value;
    
    /* check if string is empty or null */
    if (num_str == NULL || *num_str == NULL_TERMINATOR) {
        fprintf(stderr, "Error in line %d: empty or null string cannot be converted to integer\n",
                        line_number);
        return STATUS_CODE_ERR;
    }
    
    /* convert string to long */
    value = strtol(num_str, &end, BASE_10);

    /* check if conversion was successful (no remaining characters) */
    if (*end != NULL_TERMINATOR) {
        fprintf(stderr, "Error in line %d: '%s' is not a valid integer\n",
                        line_number, num_str);
        return STATUS_CODE_ERR;
    }
    
    /* validate number range */
    if (value < MIN_10_BIT_VALUE || value > MAX_10_BIT_VALUE) {
        fprintf(stderr, "Error in line %d: argument value '%ld' out of range. Must be between %d and %d\n",
                line_number, value, MIN_10_BIT_VALUE, MAX_10_BIT_VALUE);
        return STATUS_CODE_ERR;
    }
    
    /* store converted value */
    *num_int = (int)value;
    return STATUS_CODE_OK;
}


int get_mat_size(char *mat_size_str) {
    char *size_args, *size_copy, *end;
    int arg_id = 0, size = ILLEGAL_MAT_SIZE;
    long dim;  /* matrix single dimension */

    /* copy line for processing */
    size_copy = strdup(mat_size_str);
    /* tokenize with different delimiters */
    size_args = strtok(size_copy, MAT_DELIMITERS);

    while (size_args != NULL) {
        /* get dimension and convert to int */
        dim = strtol(size_args, &end, BASE_10);

        if (size == ILLEGAL_MAT_SIZE)
            size = (int)dim;  /* insert first dimension */
        else
            size *= (int)dim;  /* calculate total size */

        /* get next token/arg */
        size_args = strtok(NULL, MAT_DELIMITERS);
        arg_id++;
    }

    /* check if size is within valid range */
    if (size < MIN_MAT_SIZE || size > MAX_MAT_SIZE)
        size = ILLEGAL_MAT_SIZE;  /* illegal matrix dimensions */

    free(size_copy);
    return size;  /* return matrix dimensions */
}


void store_matrix_name(char *operand, char **mem_mat_name) {
    size_t mat_name_len;

    /* calculate matrix name length */
    mat_name_len = strchr(operand, MAT_LEFT_BRACE) - operand;

    /* allocate buffer for mat name in memory unit */
    if ((*mem_mat_name = (char *)malloc(mat_name_len + 1)) == NULL) {
        fprintf(stderr, "Error allocating memory for matrix name\n");
        return;
    }
    /* store matrix name in memory unit */
    strncpy(*mem_mat_name, operand, mat_name_len);
    (*mem_mat_name)[mat_name_len] = NULL_TERMINATOR;
}