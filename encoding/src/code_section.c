#include "../hdr/code_section.h"


int encode_op_name(MemoryUnit **mem_map,
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
            add_unit_and_increment_L(mem_map, new, L);
            
            return STATUS_CODE_OK;
        }
    }

    fprintf(stderr, "Error in line %d: unknown operation '%s'\n", line_number, op_name);
    return STATUS_CODE_ERR;  /* unknown operation */
}


int encode_operand_by_type(char *operand, int is_src, unsigned int operand_type,
                           int line_number, int *L, MemoryUnit **mem_map) {
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
            add_unit_and_increment_L(mem_map, new, L);

            /* encode row and col */
            if ((new1 = new_mem_unit()) == NULL) {  /* allocate another memory unit */
                fprintf(stderr, "Memory allocation error in line %d, operand %s\n", line_number, operand);
                return STATUS_CODE_ERR;  /* memory allocation failed */
            }

            if (encode_matrix_row_col(&new1->encoded_value, operand, line_number) == STATUS_CODE_ERR) {
                free(new1);
                return STATUS_CODE_ERR;
            }

            add_unit_and_increment_L(mem_map, new1, L);
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
        add_unit_and_increment_L(mem_map, new, L);
    }

    return STATUS_CODE_OK;  /* encoding succeeded */
}


int encode_operands(MemoryUnit **mem_map, char *op_line,
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
            if (encode_both_registers(mem_map, line_arg, line_number, L) == STATUS_CODE_ERR) {
                free(line_copy);
                return STATUS_CODE_ERR;
            }
        }
        /* operands are not both registers */
        else {
            /* extract destination operand (avoids arg override by strtok) */
            dest_arg = strtok(NULL, OP_DELIMITERS);
            
            /* encode source operand */
            if (encode_operand_by_type(line_arg, TRUE, src_operand, line_number, L, mem_map) == STATUS_CODE_ERR) {
                free(line_copy);
                return STATUS_CODE_ERR;  /* encoding failed */
            }
            
            /* encode destination operand */
            if (encode_operand_by_type(dest_arg, FALSE, dest_operand, line_number, L, mem_map) == STATUS_CODE_ERR) {
                free(line_copy);
                return STATUS_CODE_ERR;  /* encoding failed */
            }
        }
    }
    /* destination only */
    else {  /* encode destination operand */
        if (encode_operand_by_type(line_arg, FALSE, dest_operand, line_number, L, mem_map) == STATUS_CODE_ERR) {
            free(line_copy);
            return STATUS_CODE_ERR;  /* encoding failed */
        }
    }

    free(line_copy);
    return STATUS_CODE_OK;  /* encoding successful */
}


int encode_instruction(char *op_line,
                       int num_of_operands,
                       MemoryUnit **mem_map,
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
    if (encode_op_name(mem_map, op_name, dest_operand, src_operand, line_number, &L) == STATUS_CODE_ERR) {
        free(line_copy);
        free(op_name);
        return WORDS_NUM_ERROR;  /* encoding failed */
    }

    /* encode operands */
    if (encode_operands(mem_map, op_line, dest_operand,
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