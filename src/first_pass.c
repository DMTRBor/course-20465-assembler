#include "../hdr/first_pass.h"


int run_first_pass(char *filename, unsigned int *IC, unsigned int *DC,
                   MemoryUnit **mem, Label **labels) {
    int line_number = 1;
    int num_of_operands = 0;  /* number of operands for operation */
    int L = 0;  /* number of words occupied */

    int is_label = FALSE;  /* label flag */
    int is_extern = FALSE;  /* external label flag */

    /* use error cheking for content of .am file */
    int error_flag = FALSE;
    LineArg line_arg_type = ERROR;

    char am_filename[MAX_FNAME_LEN];
    FILE *fp;

    Line *curr_line = NULL;  /* lines list */
    Label *label = NULL;

    /* add macro-parsed file extension */
    strcpy(am_filename, filename);
    strcat(am_filename, ASMB_MCRO_FILE_EXTEN);

    /* open *.am file for reading */
    if ((fp = open_file(am_filename, READ_FILE_PERMISSION)) == NULL)
        return STATUS_CODE_ERR;

    /* create file lines structured list */
    /* point to first line */
    if ((curr_line = file_to_list(fp)) == NULL) {  /* memory misallocation */
        fclose(fp);
        return STATUS_CODE_ERR;
    }
    fclose(fp);  /* not needed - close file handle */

    while (curr_line != NULL) {
        line_arg_type = detect_and_validate_first_arg(curr_line->line, line_number);

        /* check if memory exceeded */
        if ((*IC + *DC) > MEMORY_SIZE) {
            fprintf(stderr, "Error in line %d: memory size exceeded\n", line_number);
            error_flag = TRUE;
            break;
        }

        switch (line_arg_type) {
            case EMPTY_LINE:
            case COMMENT:
                break;  /* ignore these lines */

            case LABEL:
                /* add to labels table */
                if ((label = new_label()) == NULL) {  /* create new label */
                    fprintf(stderr, "Memory allocation error in line %d\n", line_number);
                    error_flag = TRUE;
                    break;
                }
                
                /* detect label type */
                if (is_extern) {
                    label->name = curr_line->line;  /* label coming from extern directive */
                    set_label_fields(label, EXTERNAL, EXTERN_LABEL_ADDRESS);
                }
                else  /* set new label name */
                    label->name = get_label_name(&curr_line->line);

                /* check if label already exists */
                if (is_label_exists(labels, label)) {
                    fprintf(stderr, "Label '%s' from line %d already exists\n", label->name, line_number);
                    free(label);
                    error_flag = TRUE;
                }

                /* new label detected - add to table */
                if (add_label_to_table(labels, label) == STATUS_CODE_ERR) {
                    fprintf(stderr, "Failed to add label '%s' to list at line %d\n", label->name, line_number);
                    free(label);
                    error_flag = TRUE;
                }

                if (is_extern) {
                    is_extern = FALSE;  /* reset external label flag */
                    break;  /* go to next line */
                }
                else {
                    is_label = TRUE;
                    continue;  /* process the remaining content in the line */
                }

            case DIRECTIVE:
                if (is_label) {  /* fill label fields, if there is one */
                    set_label_fields(label, DATA, *DC);
                }
                is_label = FALSE;  /* reset label flag */

                if (is_expected_directive(curr_line->line, ENTRY_DIRECTIVE)) {
                    break;  /* .entry will be completed in second pass */
                }
                else if (is_expected_directive(curr_line->line, EXTERNAL_DIRECTIVE)) {
                    if (get_num_of_arguments(curr_line->line) != NUM_OF_EXTERN_ARGS) {
                        fprintf(stderr,
                               "Error in line %d: external directive should have %d arguments\n",
                               line_number, NUM_OF_EXTERN_ARGS);
                        error_flag = TRUE;
                        break;
                    }
                    is_extern = TRUE;  /* external label detected */
                    /* skip directive name */
                    curr_line->line += strlen(EXTERNAL_DIRECTIVE) + 1;
                    /* skip possible whitespace */
                    while (*curr_line->line && isspace((unsigned char)*curr_line->line))
                        curr_line->line++;
                    /* go to label */
                    continue;
                }
                else if (is_expected_directive(curr_line->line, DATA_DIRECTIVE)) {
                    /* encode data with following args */
                    if ((L = encode_data_direc(curr_line->line, mem, line_number)) == WORDS_NUM_ERROR) {
                        error_flag = TRUE;
                        break;
                    }
                }
                else if (is_expected_directive(curr_line->line, STRING_DIRECTIVE)) {
                    /* encode string with following args */
                    if ((L = encode_string_direc(curr_line->line, mem, line_number)) == WORDS_NUM_ERROR) {
                        error_flag = TRUE;
                        break;
                    }
                }
                else if (is_expected_directive(curr_line->line, MAT_DIRECTIVE)) {
                    /* encode mat with following args */
                    if ((L = encode_mat_direc(curr_line->line, mem, line_number)) == WORDS_NUM_ERROR) {
                        error_flag = TRUE;
                        break;
                    }
                }
                else {
                    fprintf(stderr, "Error in line %d: wrong directive name '%s'\n", line_number, curr_line->line);
                    error_flag = TRUE;
                }
                
                *DC += L;  /* update data counter */
                break;

            case INSTRUCTION:  /* instruction = operation + operands */
                /* fill label fields, if there is one */
                if (is_label) {
                    set_label_fields(label, CODE, *IC);
                }
                is_label = FALSE;  /* reset label flag */

                /* get number of operands */
                num_of_operands = get_num_of_arguments(curr_line->line);
                /* validate number of operands is correct */
                if (!is_operands_num_valid(curr_line->line, num_of_operands)) {
                    fprintf(stderr, "Error in line %d: invalid number of operands\n", line_number);
                    error_flag = TRUE;
                    break;
                }

                /* encode operation and operands */
                if ((L = encode_instruction(curr_line->line, num_of_operands,
                                            mem, line_number)) == WORDS_NUM_ERROR) {
                    error_flag = TRUE;
                    break;
                }
                
                *IC += L;  /* update instruction counter */
                break;

            default:
                fprintf(stderr, "Argument error in line %d: unrecognized argument - %s\n",
                        line_number, curr_line->line);
                error_flag = TRUE;
        }

        /* go to next line */
        curr_line = curr_line->next;
        line_number++;
    }

    /* errors found in file */
    if (error_flag) {
        free_list(curr_line);  /* free lines list */
        return STATUS_CODE_ERR;
    }

    /* separate data from code, save DCF for second pass */
    if ((*DC = update_data_labels_address(labels, *IC)) == INVALID_DCF) {
        fprintf(stderr, "Error! failed to update data labels address\n");
        free_list(curr_line);  /* free lines list */
        return STATUS_CODE_ERR;
    }

    free_list(curr_line);  /* free lines list */
    return STATUS_CODE_OK;
}