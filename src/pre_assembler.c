#include "../hdr/pre_assembler.h"


LineArg parse_line(char *line) {
    char *line_args;
    char *line_copy;
    LineArg line_arg_type = OTHER;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_args = strtok(line_copy, WHITESPACE);

    while (line_args != NULL) {
        if (strcmp(line_args, NEWLINE_STR) == STR_EQUAL) {
            line_arg_type = EMPTY_LINE;
            break;
        }
        if (strcmp(line_args, COMMENT_SIGN) == STR_EQUAL) {
            line_arg_type = COMMENT;
            break;
        }
        if (strcmp(line_args, MACRO_START) == STR_EQUAL) {
            line_arg_type = MCRO;
            break;
        }
        if (strcmp(line_args, MACRO_END) == STR_EQUAL) {
            line_arg_type = MCROEND;
            break;
        }
        /* next token/arg */
        line_args = strtok(NULL, WHITESPACE);
    }

    free(line_copy);
    return line_arg_type;
}


char* get_macro_name(char *line) {
    char *line_args;
    int arg_id = 0;  /* token index */

    /* tokenize with whitespaces */
    line_args = strtok(line, WHITESPACE);

    while (line_args != NULL) {
        /* macro name goes after mcro label */
        if (arg_id == 1)
            break;

        /* next token/arg */
        line_args = strtok(NULL, WHITESPACE);
        arg_id++;
    }

    return line_args;
}


int is_macro_call(char *line, char *macro_name) {
    char *line_args;
    char *line_copy;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_args = strtok(line, WHITESPACE);

    while (line_args != NULL) {
        /* macro called */
        if (strcmp(line_args, macro_name) == STR_EQUAL) {
            free(line_copy);
            return TRUE;
        }

        /* next token/arg */
        line_args = strtok(NULL, WHITESPACE);
    }

    free(line_copy);
    return FALSE;
}


/**
 * This function
 */
int parse_assembler_source(FILE *fp, char *filename) {
    int inside_macro = FALSE;  /* macro flag */
    LineArg line_arg_type = OTHER;
    char macro_filename[MAX_FNAME_LEN];
    Line *curr_line = NULL;
    Line *first_line = NULL;
    Macro *macro_curr_line = NULL;

    /* create file lines structured list, point to first line */
    curr_line = file_to_list(fp);
    /* point to first line in file */
    first_line = curr_line;
    /* error - line too long/memory misallocation */
    if (curr_line == NULL) {
        fclose(fp);
        return STATUS_CODE_ERR;
    }
    fclose(fp);

    /* process assembler file lines */
    while (curr_line != NULL) {
        line_arg_type = parse_line(curr_line->line);

        switch (line_arg_type) {
            case EMPTY_LINE:
            case COMMENT:
                break;  /* ignore these lines */
            case MCRO:
                inside_macro = TRUE;
                macro_curr_line = init_macro_list(get_macro_name(curr_line->line));
                delete_line_from_list(curr_line);
                break;
            case MCROEND:
                inside_macro = FALSE;
                delete_line_from_list(curr_line);
                break;
        }

        /* store macro lines in macro list */
        if (macro_curr_line != NULL && inside_macro) {
            add_line_to_macro(macro_curr_line, curr_line);
            delete_line_from_list(curr_line);
            continue;
        }

        /* found macro call */
        if (macro_curr_line != NULL && (!inside_macro) && is_macro_call(curr_line->line, macro_curr_line->name))
            insert_macro_in_list(curr_line, macro_curr_line);

        /* go to next line */
        curr_line = curr_line->next;
    }

    /* add macro-parsed file extension */
    strcpy(macro_filename, filename);
    strcat(macro_filename, ASMB_MCRO_FILE_EXTEN);

    /* write list to file after macros expansion */
    if ((fp = open_file(macro_filename, WRITE_FILE_PERMISSION)) == NULL) {
        fclose(fp);
        return STATUS_CODE_ERR;
    }

    if(list_to_file(first_line, fp) == STATUS_CODE_ERR) {
        fclose(fp);
        return STATUS_CODE_ERR;
    }

    /* free used memory */
    free_list(first_line);
    /* close file */
    fclose(fp);

    return STATUS_CODE_OK;
}