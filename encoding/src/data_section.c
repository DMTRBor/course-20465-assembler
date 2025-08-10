#include "../hdr/data_section.h"


int encode_data_direc(char *line, MemoryUnit **mem_map, int line_number) {
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

            if (encode_directive_number(mem_map, arg_val, line_number, &L) == STATUS_CODE_ERR) {
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


int encode_string_direc(char *line, MemoryUnit **mem_map, int line_number) {
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

                if (encode_directive_number(mem_map, arg_ascii, line_number, &L) == STATUS_CODE_ERR) {
                    free(line_copy);
                    return WORDS_NUM_ERROR;
                }
            }
            /* encode null terminator */
            if (encode_directive_number(mem_map, NULL_TERMINATOR_ASCII,
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


int encode_mat_direc(char *line, MemoryUnit **mem_map, int line_number) {
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

                if (encode_directive_number(mem_map, val, line_number, &L) == STATUS_CODE_ERR) {
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