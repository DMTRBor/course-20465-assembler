#include "../hdr/pre_assembler.h"


int run_pre_assembler(FILE *fp, char *filename) {
    int inside_macro = FALSE;  /* macro flag */
    int line_number = 1;
    /* no error checking for file content */
    LineArg line_arg_type = EMPTY_LINE;

    char macro_filename[MAX_FNAME_LEN];

    Line *curr_line = NULL;
    Line *first_line = NULL;

    int macro_id, macro_count = 0;
    Macro *macro_curr_line = NULL;
    Macro *macros[MAX_MACROS_ALLOWED] = { NULL };  /* array of macro lists */

    /* create file lines structured list */
    curr_line = file_to_list(fp);
    /* point to first line in file */
    first_line = curr_line;
    /* error - line too long/memory misallocation */
    if (curr_line == NULL) {
        fclose(fp);
        return STATUS_CODE_ERR;
    }
    fclose(fp);  /* not needed - close file handle */

    /* process assembler file lines */
    while (curr_line != NULL) {
        line_arg_type = detect_arg_type(curr_line->line);
        line_number++;

        switch (line_arg_type) {
            case EMPTY_LINE:
            case COMMENT:
                break;  /* ignore these lines */

            case MCRO:
                /* check if macro name is valid */
                /* check if macro header contains extraneous args */
                if (!is_macro_name_valid(curr_line->line, line_number) || !is_macro_args_num_valid(curr_line->line, NUM_OF_MCRO_ARGS)) {
                    free_list(first_line);
                    free_macro(macro_curr_line);
                    return STATUS_CODE_ERR;
                }
                inside_macro = TRUE;
                macro_curr_line = init_macro_list(get_macro_name(curr_line->line));
                /* add new macro list to array of macro lists */
                if (macro_count < MAX_MACROS_ALLOWED)
                    macros[macro_count++] = macro_curr_line;

                delete_line_from_list(curr_line);
                break;

            case MCROEND:
                /* check if macro end contains extraneous args */
                if (!is_macro_args_num_valid(curr_line->line, NUM_OF_MCROEND_ARGS)) {
                    free_list(first_line);
                    free_macro(macro_curr_line);
                    return STATUS_CODE_ERR;
                }
                inside_macro = FALSE;
                delete_line_from_list(curr_line);
                break;

            default:
                break;
        }

        /* store macro lines in macro list */
        if (macro_curr_line != NULL && inside_macro) {
            add_line_to_macro(macro_curr_line, curr_line);
            delete_line_from_list(curr_line);
            continue;
        }

        if (!inside_macro) {
            /* search for macro list in array by name */
            for (macro_id = 0; macro_id < macro_count; macro_id++) {
                if (is_macro_call(curr_line->line, macros[macro_id]->name)) {  /* found macro call */
                    insert_macro_in_list(curr_line, macros[macro_id]);
                    break;
                }
            }
        }

        /* go to next line */
        curr_line = curr_line->next;
    }

    /* add macro-parsed file extension */
    strcpy(macro_filename, filename);
    strcat(macro_filename, ASMB_MCRO_FILE_EXTEN);

    /* write list to file after macros expansion */
    if ((fp = open_file(macro_filename, WRITE_FILE_PERMISSION)) == NULL)
        return STATUS_CODE_ERR;

    if (list_to_file(first_line, fp) == STATUS_CODE_ERR) {
        fclose(fp);
        return STATUS_CODE_ERR;
    }

    /* free used memory */
    free_list(first_line);
    /* close file */
    fclose(fp);

    return STATUS_CODE_OK;
}