#include "../hdr/util.h"


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


int encode_both_registers(MemoryUnit **mem_map, char *line_arg, int line_number, int *L) {
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
    add_unit_and_increment_L(mem_map, new, L);

    return STATUS_CODE_OK;  /* encoding succeeded */
}


int encode_directive_number(MemoryUnit **mem_map, int number, int line_number, int *L) {
    MemoryUnit *new;

    if ((new = new_mem_unit()) == NULL) {  /* allocate memory unit */
        fprintf(stderr, "Memory allocation error in line %d, argument value %d\n", line_number, number);
        return STATUS_CODE_ERR;  /* memory allocation failed */
    }

    /* encode number value */
    encode_data_10_bit(&new->encoded_value, number);
    /* add new memory unit to table, increment words counter */
    add_unit_and_increment_L(mem_map, new, L);

    return STATUS_CODE_OK;
}


int encode_labels_addresses(MemoryUnit **mem_map, Label **labels) {
    MemoryUnit *mem_unit = *mem_map;
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