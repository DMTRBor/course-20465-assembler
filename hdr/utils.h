#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* one last character is null terminator */
#define MAX_FNAME_LEN 51
#define MAX_LINE_LEN 81
#define REG_NAME_LEN 2

#define IC_DC_RESET_VALUE 0

#define MIN_ALLOWED_ARGS 2

#define BASE_4 4
#define BASE_10 10

/* delimiters and hallmarks */
#define MACRO_START "mcro"
#define MACRO_END "mcroend"
#define LABEL_SIGN ":"
#define INSTRUCTION_SIGN "."
#define DIRECT_ADDR_SIGN "#"
#define REGISTER_PREFIX "r"
#define MATRIX_LEFT_BRACE "["
#define MATRIX_RIGHT_BRACE "]"
#define COMMENT_SIGN ';'
#define NEWLINE_STR "\n"

#define WHITESPACE " \t"
#define NEWLINE_CHAR '\n'
#define NULL_TERMINATOR '\0'
#define UNDERSCORE '_'
#define COMMA ','
#define ZERO '0'

#define STR_EQUAL 0

#define READ_FILE_PERMISSION "r"
#define WRITE_FILE_PERMISSION "w"

/* file extensions */
#define ASMB_FILE_EXTEN ".as"
#define ASMB_MCRO_FILE_EXTEN ".am"
#define OBJECT_FILE_EXTEN ".ob"
#define EXTERNAL_FILE_EXTEN ".ext"
#define ENTRIES_FILE_EXTEN ".ent"


enum {
    STATUS_CODE_OK,
    STATUS_CODE_ERR
};


enum {
    FALSE,
    TRUE
};


typedef enum {
    EMPTY_LINE,
    COMMENT,
    MCRO,
    MCROEND,
    LABEL,
    OPERATION,
    INSTRUCTION,
    DIRECT_ADDR,
    REGISTER,
    MATRIX,
    ERROR
} LineArg;


/* forward declarations */
void decimal_to_base4(char *, char *);

long file_content_size(FILE*);
FILE* open_file(char *, char *);
int line_too_long(char *, FILE *);

LineArg detect_arg_type(char *);

char* get_macro_name(char *);
int is_macro_call(char *, char *);

#endif