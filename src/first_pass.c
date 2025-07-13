#include "../hdr/first_pass.h"


int run_first_pass(char *filename, int *ic, int *dc) {
    int line_number = 1;
    int error_flag = FALSE;

    LineArg line_arg_type = OTHER;

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
        line_arg_type = detect_arg_type(curr_line->line);
        line_number++;

        // switch (line_arg_type) {
        //     case EMPTY_LINE:
        //     case COMMENT:
        //         break;  /* ignore these lines */

        //     case OTHER: {
        //         /* process other lines */
        //         if (process_other_line(curr_line->line, &mem, labels, ic, dc) == STATUS_CODE_ERR) {
        //             error_flag = TRUE;
        //         }
        //         break;
        //     }

        //     case LABEL: {
        //         /* process label line */
        //         if (process_label_line(curr_line->line, &mem, labels, ic) == STATUS_CODE_ERR) {
        //             error_flag = TRUE;
        //         }
        //         break;
        //     }

        //     case INSTRUCTION: {
        //         /* process instruction line */
        //         if (process_instruction_line(curr_line->line, &mem, labels, ic) == STATUS_CODE_ERR) {
        //             error_flag = TRUE;
        //         }
        //         break;
        //     }

        //     default:
        //         fprintf(stderr, "Error in line %d: unrecognized line type\n", line_number);
        //         error_flag = TRUE;
        // }

        /* go to next line */
        curr_line = curr_line->next;
    }
    
    /* free used memory */
    free_list(curr_line);

    return STATUS_CODE_OK;
}