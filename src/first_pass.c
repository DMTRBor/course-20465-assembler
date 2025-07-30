#include "../hdr/first_pass.h"


int run_first_pass(char *filename, int *IC, int *DC, MemoryUnit **mem, Label **labels) {
    int line_number = 1;
    int num_of_operands = 0;  /* number of operands for operation */
    /* number of words occupied by the operation and the operands */
    int L = 0;
    int is_label = FALSE;  /* label flag */

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
                /* fill label fields */
                set_label_fields(label, line_number,
                                 get_label_name(&curr_line->line), *IC);

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

                is_label = TRUE;
                label = label->next;  /* point to next label */
                continue;             /* process the remaining content in the line */

            case DIRECTIVE:
                /* .entry will be completed in second pass */
                // if (is_extern()) {
                    /* add label after .extern to labels table */
                    /* set type of label as external and value as 0 */
                // }
                break;

            case INSTRUCTION:
                /* validate operands number */
                if ((num_of_operands = get_num_of_operands(curr_line->line, line_number)) == OPERANDS_NUM_ERROR) {
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
                fprintf(stderr, "Argument error in line %d: unrecognized argument\n", line_number);
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
    
    /* free used memory */
    free_list(curr_line);

    return STATUS_CODE_OK;
}