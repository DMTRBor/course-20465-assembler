#include "../hdr/pre_assembler.h"


LineArg parse_line(char *line) {
    char *line_args;
    char *line_copy;
    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_args = strtok(line_copy, WHITESPACE);

    while (line_args != NULL) {
        if (strcmp(line_args, NEWLINE_STR) == STR_EQUAL)
            return EMPTY_LINE;

        if (strcmp(line_args, COMMENT_SIGN) == STR_EQUAL)
            return COMMENT;

        if (strcmp(line_args, MACRO_START) == STR_EQUAL)
            return MCRO;

        if (strcmp(line_args, MACRO_END) == STR_EQUAL)
            return MCROEND;

        line_args = strtok(NULL, WHITESPACE);
    }

    free(line_copy);
    return OTHER;
}


/**
 * This function
 */
int parse_assembler_source(FILE *fp, char *filename) {
    char macro_filename[MAX_FNAME_LEN];
    Line *lines_list;
    Macro *macro_list;
    LineArg line_arg_type;

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
                break;
            case MCROEND:
                break;
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