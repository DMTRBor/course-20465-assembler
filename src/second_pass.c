#include "../hdr/second_pass.h"


int run_second_pass(char *filename, unsigned int *IC, unsigned int *DC,
                    MemoryUnit **mem, Label **labels) {
    int line_number = 1;

    /* use error cheking for content of .am file */
    int error_flag = FALSE;
    LineArg line_arg_type = ERROR;

    char am_filename[MAX_FNAME_LEN];
    FILE *fp;

    Line *curr_line = NULL;  /* lines list */
    char *label_name;

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

        switch (line_arg_type) {
            case EMPTY_LINE:
            case COMMENT:
                break;  /* ignore these lines */

            case LABEL:
                /* skip label name */
                curr_line->line = strchr(curr_line->line, LABEL_END_SIGN);
                curr_line->line++;  /* skip label end sign */

                /* skip whitespaces */
                while (*curr_line->line && isspace((unsigned char)*curr_line->line))
                    curr_line->line++;

                /* continue to the rest of the line */
                continue;

            case DIRECTIVE:
                /* entry detected */
                if (is_expected_directive(curr_line->line, ENTRY_DIRECTIVE)) {
                    /* extract label name */
                    label_name = strchr(curr_line->line, WHITESPACE_CHAR);
                    /* skip whitespaces */
                    while (*label_name && isspace((unsigned char)*label_name))
                        label_name++;

                    /* check if label is defined */
                    if (!is_label_exists(labels, label_name)) {
                        fprintf(stderr, "Label '%s' from line %d does not exist in labels table\n", label_name, line_number);
                        error_flag = TRUE;
                    }

                    /* change label type to ENTRY */
                    set_label_type(labels, label_name, ENTRY);
                }
                /* skip data, string, mat and extern */
                break;

            case INSTRUCTION:
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

    /* build output files */

    free_list(curr_line);  /* free lines list */
    return STATUS_CODE_OK;
}