#include "../hdr/encoding.h"


void encode_operand_8_bit(Word *word, int operand_value) {
    /* write the 8-bit value to fields starting from msb */
    word->dest_operand =  UINT_BITS_TO_DEST(operand_value);
    word->src_operand  =  UINT_BITS_TO_SRC(operand_value);
    word->opcode       =  UINT_BITS_TO_OPCODE(operand_value);
}


void encode_data_10_bit(Word *word, int data_value) {
    /* write the 10-bit value to all Word fields */
    word->encoding_type = INT_BITS_TO_ENC_TYPE(data_value);
    word->dest_operand  = INT_BITS_TO_DEST(data_value);
    word->src_operand   = INT_BITS_TO_SRC(data_value);
    word->opcode        = INT_BITS_TO_OPCODE(data_value);
}


void encode_operand_first_index(Word *word, unsigned int index) {
    /* first index value */
    word->opcode = UINT_TO_UPPER_4_BITS(index);
}


void encode_operand_sec_index(Word *word, unsigned int index) {
    /* upper 2 bits of second index value */
    word->src_operand = UINT_BITS_TO_SRC(index);
    /* lower 2 bits of second index value */
    word->dest_operand = UINT_BITS_TO_DEST(index);
}


int encode_matrix_row_col(Word *word, char *operand, int line_number) {
    char *op_args, *op_copy;
    int arg_id = 0;  /* arg index */

    /* copy line for processing */
    op_copy = strdup(operand);
    /* tokenize with different delimiters */
    op_args = strtok(op_copy, MAT_DELIMITERS);

    while (op_args != NULL) {
        if (arg_id > FIRST_ARG) {  /* skip matrix name */
            /* validate register row/col index */
            if (!is_register(op_args)) {
                fprintf(stderr, "Error in line %d: invalid matrix index '%s'\n", line_number, op_args);
                free(op_copy);
                return STATUS_CODE_ERR;  /* invalid row/col */
            }

            if (arg_id == MAT_FIRST_INDEX) {
                /* encode row index */
                encode_operand_first_index(word, (unsigned int)strtoul(op_args + 1, NULL, BASE_10));
            }
            else if (arg_id == MAT_SECOND_INDEX) {
                /* encode column index */
                encode_operand_sec_index(word, (unsigned int)strtoul(op_args + 1, NULL, BASE_10));
            }
            else {
                fprintf(stderr, "Error in line %d: too many matrix indices\n", line_number);
                free(op_copy);
                return STATUS_CODE_ERR;  /* too many indices (not 2D matrix) */
            }
        }

        /* get next token/arg */
        op_args = strtok(NULL, MAT_DELIMITERS);
        arg_id++;
    }

    free(op_copy);
    return STATUS_CODE_OK;
}


int encode_both_registers(MemoryUnit **table, char *line_arg, int line_number, int *L) {
    MemoryUnit *new;

    /* validate first register */
    if (!is_register(line_arg)) {
        fprintf(stderr, "Error in line %d: invalid register number '%s'\n", line_number, line_arg);
        return STATUS_CODE_ERR;  /* invalid register */
    }

    /* allocate new memory unit */
    if ((new = new_mem_unit()) == NULL) {
        fprintf(stderr, "Memory allocation error in line %d, operand %s\n", line_number, line_arg);
        return STATUS_CODE_ERR;  /* memory allocation failed */
    }

    new->encoded_value.encoding_type = A;
    encode_operand_first_index(&new->encoded_value,
                               (unsigned int)strtoul(line_arg + 1, NULL, BASE_10));

    /* get next operand */
    line_arg = strtok(NULL, OP_DELIMITERS);
    /* validate second register */
    if (!is_register(line_arg)) {
        fprintf(stderr, "Error in line %d: invalid register number '%s'\n", line_number, line_arg);
        return STATUS_CODE_ERR;  /* invalid register */
    }
    
    encode_operand_sec_index(&new->encoded_value,
                            (unsigned int)strtoul(line_arg + 1, NULL, BASE_10));
    /* add new memory unit to table, increment words counter */
    add_unit_and_increment_L(table, new, L);

    return STATUS_CODE_OK;  /* encoding succeeded */
}


int encode_directive_number(MemoryUnit **table, int number, int line_number, int *L) {
    MemoryUnit *new;

    if ((new = new_mem_unit()) == NULL) {  /* allocate memory unit */
        fprintf(stderr, "Memory allocation error in line %d, argument value %d\n", line_number, number);
        return STATUS_CODE_ERR;  /* memory allocation failed */
    }

    /* encode number value */
    encode_data_10_bit(&new->encoded_value, number);
    /* add new memory unit to table, increment words counter */
    add_unit_and_increment_L(table, new, L);

    return STATUS_CODE_OK;
}


int encode_labels_addresses(MemoryUnit **mem, Label **labels) {
    MemoryUnit *mem_unit = *mem;
    Label *label;

    while (mem_unit != NULL) {
        if (mem_unit->label_op_name != NULL) {  /* label found */
            label = *labels;
            /* search for label in labels table */
            while (label != NULL) {
                if (strcmp(mem_unit->label_op_name, label->name) == STR_EQUAL) {  /* label found */
                    /* check if external label */
                    if (label->type == EXTERNAL)
                        mem_unit->encoded_value.encoding_type = E;  /* external label */

                    /* encode address */
                    encode_operand_8_bit(&mem_unit->encoded_value, label->address);
                    break;  /* stop label search */
                }
                label = label->next;
            }

            if (label == NULL) {  /* label not found in whole table */
                fprintf(stderr, "Error: label '%s' not found in labels table\n",
                        mem_unit->label_op_name);
                return STATUS_CODE_ERR;
            }
        }
        mem_unit = mem_unit->next;
    }

    return STATUS_CODE_OK;  /* encoding labels addresses succeeded */
}


/* --------------------------------- Code ---------------------------------- */

int encode_op_name(MemoryUnit **table,
                   char *op_name,
                   unsigned int dest_operand,
                   unsigned int src_operand,
                   int line_number,
                   int *L) {
    int id;  /* array index */
    unsigned int encoding_type, opcode;
    MemoryUnit *new;

    /* search for operation in operations table */
    for (id = 0; id < NUM_OF_OPERATIONS; id++) {
        if (strcmp(op_name, operations[id].name) == STR_EQUAL) {
            /* allocate new memory unit */
            if ((new = new_mem_unit()) == NULL) {
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
            /* add to table and count word */
            add_unit_and_increment_L(table, new, L);
            
            return STATUS_CODE_OK;
        }
    }

    fprintf(stderr, "Error in line %d: unknown operation '%s'\n", line_number, op_name);
    return STATUS_CODE_ERR;  /* unknown operation */
}


int encode_operand_by_type(char *operand, int is_src, unsigned int operand_type,
                           int line_number, int *L, MemoryUnit **table) {
    MemoryUnit *new;   /* for first word */
    MemoryUnit *new1;  /* for second word */

    /* allocate new memory unit */
    if ((new = new_mem_unit()) == NULL) {
        fprintf(stderr, "Memory allocation error in line %d, operand %s\n", line_number, operand);
        return STATUS_CODE_ERR;  /* memory allocation failed */
    }

    /* source or destination depends on flag is_src*/
    switch (operand_type) {
        case IMMEDIATE:
            /* check if operand value is legal */
            if (is_operand_value_legal(operand + 1, line_number) == FALSE) {
                fprintf(stderr, "Error in line %d: illegal immediate operand value '%s'\n", line_number, operand);
                return STATUS_CODE_ERR;  /* illegal value */
            }

            new->encoded_value.encoding_type = A;
            /* encode immediate value */
            encode_operand_8_bit(&new->encoded_value,
                                (int)strtol(operand + 1, NULL, BASE_10));
            break;
        case DIRECT:
            /* no encoding at first pass - store label name */
            new->label_op_name = strdup(operand);
            new->encoded_value.encoding_type = R;
            break;
        case MAT:
            /* no encoding at first pass - store label name */
            store_matrix_name(operand, &new->label_op_name);
            new->encoded_value.encoding_type = R;
            add_unit_and_increment_L(table, new, L);

            /* encode row and col */
            if ((new1 = new_mem_unit()) == NULL) {  /* allocate another memory unit */
                fprintf(stderr, "Memory allocation error in line %d, operand %s\n", line_number, operand);
                return STATUS_CODE_ERR;  /* memory allocation failed */
            }

            if (encode_matrix_row_col(&new1->encoded_value, operand, line_number) == STATUS_CODE_ERR) {
                free(new1);
                return STATUS_CODE_ERR;
            }

            add_unit_and_increment_L(table, new1, L);
            break;
        case REG:
            if (!is_register(operand)) {
                fprintf(stderr, "Error in line %d: invalid register number '%s'\n", line_number, operand);
                return STATUS_CODE_ERR;  /* invalid register */
            }

            new->encoded_value.encoding_type = A;
            if (is_src) {  /* source operand is a register */
                encode_operand_first_index(&new->encoded_value,
                                          (unsigned int)strtoul(operand + 1, NULL, BASE_10));
            }
            else {  /* destination operand is a register */
                encode_operand_sec_index(&new->encoded_value,
                                        (unsigned int)strtoul(operand + 1, NULL, BASE_10));
            }
            break;
        default:
            fprintf(stderr, "Error in line %d: unknown operand encoding type\n", line_number);
            return STATUS_CODE_ERR;  /* unknown operand type */
    }

    /* matrix addition to table done previously*/
    if (operand_type != MAT) {
        /* add new memory unit to table, increment words counter */
        add_unit_and_increment_L(table, new, L);
    }

    return STATUS_CODE_OK;  /* encoding succeeded */
}


int encode_operands(MemoryUnit **table, char *op_line,
                    unsigned int dest_operand, unsigned int src_operand,
                    int line_number, int *L, int num_of_operands) {
    char *line_arg, *line_copy, *dest_arg;
    
    /* no operands, no need to encode */
    if (num_of_operands == NO_OPERANDS) {
        return STATUS_CODE_OK;
    }

    /* copy line for processing */
    line_copy = strdup(op_line);
    /* tokenize with different delimiters */
    line_arg = strtok(line_copy, OP_DELIMITERS);
    /* cut operation name */
    line_arg = strtok(NULL, OP_DELIMITERS);
    
    /* source and destination */
    if (num_of_operands == SRC_AND_DEST) {
        if (src_operand == REG && dest_operand == REG) {  /* both operands are registers */
            if (encode_both_registers(table, line_arg, line_number, L) == STATUS_CODE_ERR) {
                free(line_copy);
                return STATUS_CODE_ERR;
            }
        }
        /* operands are not both registers */
        else {
            /* extract destination operand (avoids arg override by strtok) */
            dest_arg = strtok(NULL, OP_DELIMITERS);
            
            /* encode source operand */
            if (encode_operand_by_type(line_arg, TRUE, src_operand, line_number, L, table) == STATUS_CODE_ERR) {
                free(line_copy);
                return STATUS_CODE_ERR;  /* encoding failed */
            }
            
            /* encode destination operand */
            if (encode_operand_by_type(dest_arg, FALSE, dest_operand, line_number, L, table) == STATUS_CODE_ERR) {
                free(line_copy);
                return STATUS_CODE_ERR;  /* encoding failed */
            }
        }
    }
    /* destination only */
    else {  /* encode destination operand */
        if (encode_operand_by_type(line_arg, FALSE, dest_operand, line_number, L, table) == STATUS_CODE_ERR) {
            free(line_copy);
            return STATUS_CODE_ERR;  /* encoding failed */
        }
    }

    free(line_copy);
    return STATUS_CODE_OK;  /* encoding successful */
}


int encode_instruction(char *op_line,
                       int num_of_operands,
                       MemoryUnit **table,
                       int line_number) {
    char *line_arg, *line_copy, *op_name;
    LineArg operand_type;

    /* word fields -initialize with zeros */
    unsigned int dest_operand = IMMEDIATE;
    unsigned int src_operand = IMMEDIATE;

    /* words and args counter */
    int L = 0, arg_id = 0;

    /* copy line for processing */
    line_copy = strdup(op_line);
    /* tokenize with different delimiters */
    line_arg = strtok(line_copy, OP_DELIMITERS);
    
    /* extract info for operation encoding */
    while (line_arg != NULL) {
        if (arg_id == FIRST_ARG)  /* get operation name */
            op_name = strdup(line_arg);
        else {
            if ((operand_type = get_operand_type(line_arg)) == ERROR) {
                fprintf(stderr, "Error in line %d: invalid operand type - %s\n", line_number, line_arg);
                free(line_copy);
                free(op_name);
                return WORDS_NUM_ERROR;  /* invalid operand type */
            }

            /* set operand field based on type */
            set_word_operand_field(operand_type, arg_id, num_of_operands,
                                   &dest_operand, &src_operand);
        }

        /* get next token/arg */
        line_arg = strtok(NULL, OP_DELIMITERS);
        arg_id++;
    }

    /* validate addressing methods */
    if (!is_legal_addressing_methods(op_name, dest_operand, src_operand, num_of_operands)) {
        fprintf(stderr, "Error in line %d: illegal addressing methods for operation %s\n", line_number, op_name);
        free(line_copy);
        free(op_name);
        return WORDS_NUM_ERROR;  /* encoding failed */
    }

    /* encode operation name */
    if (encode_op_name(table, op_name, dest_operand, src_operand, line_number, &L) == STATUS_CODE_ERR) {
        free(line_copy);
        free(op_name);
        return WORDS_NUM_ERROR;  /* encoding failed */
    }

    /* encode operands */
    if (encode_operands(table, op_line, dest_operand,
                        src_operand, line_number, &L, num_of_operands) == STATUS_CODE_ERR) {
        free(line_copy);
        free(op_name);
        return WORDS_NUM_ERROR;  /* encoding failed */
    }

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


/* --------------------------------- Data ---------------------------------- */

int encode_data_direc(char *line, MemoryUnit **table, int line_number) {
    char *line_arg, *line_copy;
    /* words and args counter */
    int L = 0, arg_id = 0, arg_val = MIN_10_BIT_VALUE;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with different delimiters */
    line_arg = strtok(line_copy, OP_DELIMITERS);

    while (line_arg != NULL) {
        if (arg_id > FIRST_ARG) {  /* skip directive name */
            if (to_int(line_arg, &arg_val, line_number) == STATUS_CODE_ERR) {
                free(line_copy);
                return WORDS_NUM_ERROR;
            }

            if (encode_directive_number(table, arg_val, line_number, &L) == STATUS_CODE_ERR) {
                free(line_copy);
                return WORDS_NUM_ERROR;
            }
        }

        /* get next token/arg */
        line_arg = strtok(NULL, OP_DELIMITERS);
        arg_id++;
    }

    free(line_copy);

    /* check if number of words is not exceeded */
    if (L > MAX_WORDS_IN_SENTENCE) {            
        fprintf(stderr, "Error in line %d: too many words in sentence\n", line_number);
        return WORDS_NUM_ERROR;  /* too many words */
    }

    return L;  /* return number of words encoded */
}


int encode_string_direc(char *line, MemoryUnit **table, int line_number) {
    char *line_arg, *line_copy;
    /* words and args counter */
    int L = 0, arg_id = 0, arg_ascii = 0, ch_id;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with different delimiters */
    line_arg = strtok(line_copy, OP_DELIMITERS);

    while (line_arg != NULL) {
        if (arg_id > FIRST_ARG) {  /* skip directive name */
            /* encode characters in the string */
            for (ch_id = 0; line_arg[ch_id] != NULL_TERMINATOR; ch_id++) {
                if (line_arg[ch_id] == QUOTATION_MARK) {
                    continue;  /* skip quotation marks */
                }

                /* 7/8 bits ASCII, will not exceed 10 bits */
                /* get character to ascii value */
                arg_ascii = (int)line_arg[ch_id];

                if (encode_directive_number(table, arg_ascii, line_number, &L) == STATUS_CODE_ERR) {
                    free(line_copy);
                    return WORDS_NUM_ERROR;
                }
            }
            /* encode null terminator */
            if (encode_directive_number(table, NULL_TERMINATOR_ASCII,
                                        line_number, &L) == STATUS_CODE_ERR) {
                free(line_copy);
                return WORDS_NUM_ERROR;
            }
        }

        /* get next token/arg */
        line_arg = strtok(NULL, OP_DELIMITERS);
        arg_id++;
    }

    free(line_copy);

    /* each word encoded independently - no limit on total words */
    return L;  /* return number of words encoded */
}


int encode_mat_direc(char *line, MemoryUnit **table, int line_number) {
    char *line_arg, *line_copy, *mat_vals;
    /* words and args counter */
    int L = 0, arg_id = 0;
    int val = MIN_10_BIT_VALUE, mat_size, mat_id = 0; /* for matrix */

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with different delimiters */
    line_arg = strtok(line_copy, OP_DELIMITERS);

    while (line_arg != NULL) {
        if (arg_id > FIRST_ARG) {  /* skip directive name */
            if (line_arg[0] == MAT_LEFT_BRACE) {  /* matrix dimensions detected */
                if ((mat_size = get_mat_size(line_arg)) == ILLEGAL_MAT_SIZE) {
                    fprintf(stderr, "Error in line %d: illegal matrix size '%d'\n", line_number, mat_size);
                    free(line_copy);
                    return WORDS_NUM_ERROR;  /* illegal matrix size */
                }
            }

            /* too many values for matrix provided */
            if (mat_id == mat_size) {
                fprintf(stderr, "Error in line %d: too many values for matrix provided\n", line_number);
                free(line_copy);
                free(mat_vals);
                return WORDS_NUM_ERROR;
            }

            /* copy for matrix processing (avoids token override) */
            mat_vals = strdup(line);
            mat_vals = strtok(NULL, OP_DELIMITERS);  /* skip directive name */
            mat_vals = strtok(NULL, OP_DELIMITERS);  /* skip matrix dimensions */

            /* allocate memory for matrix values */
            for (mat_id = 0; mat_id < mat_size; mat_id++) {
                /* try to get next token/arg */
                if (mat_vals != NULL) {
                    if (to_int(mat_vals, &val, line_number) == STATUS_CODE_ERR) {
                        free(line_copy);
                        free(mat_vals);
                        return WORDS_NUM_ERROR;  /* conversion error */
                    }
                    /* go to next value */
                    mat_vals = strtok(NULL, OP_DELIMITERS);
                }
                else  /* no values were provided, allocate and fill with zeros */
                    val = 0;

                if (encode_directive_number(table, val, line_number, &L) == STATUS_CODE_ERR) {
                    free(line_copy);
                    free(mat_vals);
                    return WORDS_NUM_ERROR;  /* encoding failed */
                }
            }
        }

        /* get next token/arg */
        line_arg = strtok(NULL, OP_DELIMITERS);
        arg_id++;
    }

    free(line_copy);
    free(mat_vals);

    /* check if number of words is not exceeded */
    if (L > MAX_WORDS_IN_SENTENCE) {            
        fprintf(stderr, "Error in line %d: too many words in sentence\n", line_number);
        return WORDS_NUM_ERROR;  /* too many words */
    }

    return L;  /* return number of words encoded */
}