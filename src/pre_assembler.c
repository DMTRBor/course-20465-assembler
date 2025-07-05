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
    char *line_copy;
    int arg_id = 0;  /* token index */

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_args = strtok(line_copy, WHITESPACE);

    while (line_args != NULL) {
        /* macro name goes after mcro label */
        if (arg_id == 1)
            break;

        /* next token/arg */
        line_args = strtok(NULL, WHITESPACE);
        arg_id++;
    }

    free(line_copy);
    return line_args;
}


/**
 * This function
 */
int parse_assembler_source(FILE *fp, char *filename) {
    char macro_filename[MAX_FNAME_LEN];
    Line *lines_list;
    Macro *macro_list;
    LineArg line_arg_type;
    int inside_macro = FALSE;  /* macro flag */

    /* create file lines structured list */
    lines_list = file_to_list(fp);
    /* error - line too long/memory misallocation */
    if (lines_list == NULL) {
        fclose(fp);
        return STATUS_CODE_ERR;
    }
    fclose(fp);

    /* process assembler file lines */
    while (lines_list != NULL) {
        line_arg_type = parse_line(lines_list->line);

        switch (line_arg_type) {
            case EMPTY_LINE:
            case COMMENT:
                break;  /* ignore these lines */
            case MCRO:
                inside_macro = TRUE;
                macro_list = init_macro_list(get_macro_name(lines_list->line));
                break;
            case MCROEND:
                inside_macro = FALSE;
                break;
        }

        /* encountered mcroend */
        if (macro_list != NULL) {
            if (inside_macro)  /* inside macro */
                add_line_to_macro(macro_list, lines_list);    

            delete_line();
        }

        /* go to next line */
        lines_list = lines_list->next;
    }

    /* add macro-parsed file extension */
    strcpy(macro_filename, filename);
    strcat(macro_filename, ASMB_MCRO_FILE_EXTEN);

    /* write list to file after macros expansion */
    if ((fp = open_file(filename, WRITE_FILE_PERMISSION)) == NULL) {
        fclose(fp);
        return STATUS_CODE_ERR;
    }

    if(list_to_file(lines_list, fp) == STATUS_CODE_ERR) {
        fclose(fp);
        return STATUS_CODE_ERR;
    }

    /* finish - close file and free used memory */
    free_list(lines_list);
    fclose(fp);
    return STATUS_CODE_OK;
}