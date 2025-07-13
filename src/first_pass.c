#include "../hdr/first_pass.h"


int run_first_pass(char *filename, int *ic, int *dc) {
    int line_number = 1;
    /* use error cheking for content of .am file */
    int error_flag = FALSE;
    LineArg line_arg_type = ERROR;

    int is_label = FALSE;  /* label flag */

    /* Initialize ic and dc */
    *ic = IC_INITIAL_VALUE;
    *dc = DC_INITIAL_VALUE;

    char am_filename[MAX_FNAME_LEN];
    FILE *fp;

    Line *curr_line = NULL;  /* lines list */
    MemoryUnit *mem = NULL;  /* memory map */
    Label *labels[MAX_LABELS_ALLOWED] =  { NULL };  /* labels map */

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
                break;

            case OPERATION:
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