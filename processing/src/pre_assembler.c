#include "../hdr/pre_assembler.h"


int run_pre_assembler(FILE *fp, char *filename) {
    int inside_macro = FALSE;  /* macro flag */
    int line_number = 1;  /* start at first line */
    LineArg line_arg_type = EMPTY_LINE;

	/* use error cheking for content of .as file */
	int error_flag = FALSE;

    char macro_filename[MAX_FNAME_LEN];

    Line *curr_line = NULL;
    Line *first_line = NULL;

    int macro_id, macro_count = 0;  /* count number of macros */
    Macro *macro_curr_line = NULL;  /* macro lines list */
    Macro *macros[MAX_MACROS_ALLOWED] = { NULL };  /* array of macro lists */

    /* create file lines structured list */
    curr_line = file_to_lines_list(fp);
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
				if (!is_macro_name_valid(curr_line->line, line_number))
                    error_flag = TRUE;

                /* check if macro header contains extraneous args */
                if (!is_macro_args_num_valid(curr_line->line, NUM_OF_MCRO_ARGS)) {
                    fprintf(stderr, "Error in line %d: extraneous argument in 'mcro' line: %s\n", line_number, curr_line->line);
                    error_flag = TRUE;
                }
                inside_macro = TRUE;

                if((macro_curr_line = init_macro_list(get_macro_name(curr_line->line))) == NULL)
                    error_flag = TRUE;  /* failed to create macro lines list */
                
                /* add new macro list to array of macro lists */
                if (macro_count < MAX_MACROS_ALLOWED)
                    macros[macro_count++] = macro_curr_line;
				else {
                    fprintf(stderr, "Error - exceeded maximum of %d macros allowed\n", MAX_MACROS_ALLOWED);
                    error_flag = TRUE;
                }

                delete_line_from_list(curr_line);
                break;

            case MCROEND:
                /* check if macro end contains extraneous args */
                if (!is_macro_args_num_valid(curr_line->line, NUM_OF_MCROEND_ARGS)) {
                    fprintf(stderr, "Error in line %d: extraneous argument in 'mcroend' line: %s\n", line_number, curr_line->line);
                    error_flag = TRUE;
                }
                inside_macro = FALSE;
                delete_line_from_list(curr_line);
                break;

            default:
                break;
        }

        /* store macro lines in macro list */
        if (macro_curr_line != NULL && inside_macro) {
            if (add_line_to_macro(macro_curr_line, curr_line) == STATUS_CODE_ERR)
                error_flag = TRUE;
            delete_line_from_list(curr_line);
            continue;
        }

        if (!inside_macro) {
            /* search for macro list in array by name */
            for (macro_id = 0; macro_id < macro_count; macro_id++) {
                if (is_macro_call(curr_line->line, macros[macro_id]->name)) {  /* found macro call */
                    if (insert_macro_in_list(curr_line, macros[macro_id]) == STATUS_CODE_ERR)
                        error_flag = TRUE;  /* failed to insert macro lines on caller line */
                    
                    break;
                }  /* end is_macro_call */
            }  /* end for */
        }  /* end inside_macro condition check */

        /* go to next line */
        curr_line = curr_line->next;
    }

	/* errors found in file */
    if (error_flag) {
        free_lines_list(first_line);
        free_macro(macro_curr_line);
        return STATUS_CODE_ERR;
    }

    /* add macro-parsed file extension */
    strcpy(macro_filename, filename);
    strcat(macro_filename, ASMB_MACRO_FILE_EXTEN);

    /* write list to file after macros expansion */
    if ((fp = open_file(macro_filename, WRITE_FILE_PERMISSION)) == NULL) {
        free_lines_list(first_line);  /* free used memory */
        free_macro(macro_curr_line);
        return STATUS_CODE_ERR;
    }

    if (lines_list_to_file(first_line, fp) == STATUS_CODE_ERR) {
        free_lines_list(first_line);  /* free used memory */
        free_macro(macro_curr_line);
        fclose(fp);
        return STATUS_CODE_ERR;
    }

    /* free used memory */
    free_lines_list(first_line);
	free_macro(macro_curr_line);
    /* close file */
    fclose(fp);

    return STATUS_CODE_OK;
}
