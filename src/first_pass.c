#include "../hdr/first_pass.h"


int run_first_pass(char *filename, int *ic, int *dc, MemoryUnit *mem, Label *labels) {
    int line_number = 1;
    /* use error cheking for content of .am file */
    int error_flag = FALSE;
    LineArg line_arg_type = ERROR;

    int is_label = FALSE;  /* label flag */

    char am_filename[MAX_FNAME_LEN];
    FILE *fp;

    Line *curr_line = NULL;  /* lines list */
    Label *curr_label = NULL;  /* current label */

    /* add macro-parsed file extension */
    strcpy(am_filename, filename);
    strcat(am_filename, ASMB_MCRO_FILE_EXTEN);

    /* open *.am file for reading */
    if ((fp = open_file(am_filename, READ_FILE_PERMISSION)) == NULL)
        return STATUS_CODE_ERR;

    /* create file lines structured list */
    /* point to first line */
    curr_line = file_to_list(fp);

    if (curr_line == NULL) {  /* memory misallocation */
        fclose(fp);
        return STATUS_CODE_ERR;
    }
    fclose(fp);  /* not needed - close file handle */

    while (curr_line != NULL) {
        line_arg_type = detect_and_validate_first_arg(curr_line->line, line_number);
        line_number++;

        switch (line_arg_type) {
            case EMPTY_LINE:
            case COMMENT:
                break;  /* ignore these lines */

            case LABEL:
                is_label = TRUE;
                /* add to labels table */

                break;

            case INSTRUCTION:
                /* .entry will be completed in second pass */
                if (is_extern()) {
                    /* add label after .extern to labels table */
                    /* set type of label as external and value as 0 */
                }
                break;

            case OPERATION:
                // if (is_label) {
                    // /* add label to labels table */
                    // curr_label = init_label(curr_line->line, CODE, *ic);
                    // if (curr_label == NULL) {
                    //     fprintf(stderr, "Memory allocation error in line %d\n", line_number);
                    //     error_flag = TRUE;
                    //     break;
                    // }
                    // /* check if label already exists */
                    // if (add_label_to_table(labels, curr_label, *ic) == STATUS_CODE_ERR) {
                    //     fprintf(stderr, "Label '%s' already exists in line %d\n", curr_label->name, line_number);
                    //     free(curr_label);
                    //     error_flag = TRUE;
                    // }
                // }

                /* parse operands */
                

                /* encode operation and operands */

                break;

            default:
                fprintf(stderr, "Argument error in line %d: unrecognized argument\n", line_number);
                error_flag = TRUE;
        }

        /* go to next line */
        curr_line = curr_line->next;
    }
    
    /* free used memory */
    free_list(curr_line);

    return STATUS_CODE_OK;
}