#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "../hdr/machine.h"

/* one last character is null terminator */
#define MAX_FNAME_LEN 51
#define MAX_LINE_LEN 81
#define REG_NAME_LEN 2

#define MIN_ALLOWED_ARGS 2

#define BASE_4 4
#define BASE_10 10

#define STR_EQUAL 0

/* delimiters and hallmarks */
#define MACRO_START "mcro"
#define MACRO_END "mcroend"
#define LABEL_SIGN ":"
#define INSTRUCTION_SIGN "."
#define IMMEDIATE_ADDR_SIGN '#'
#define REGISTER_PREFIX "r"
#define MAT_LEFT_BRACE '['
#define MAT_RIGHT_BRACE ']'
#define COMMENT_SIGN ';'
#define NEWLINE_STR "\n"

#define WHITESPACE " \t"
#define OP_DELIMITERS " \t\r,"
#define NEWLINE_CHAR '\n'
#define NULL_TERMINATOR '\0'
#define UNDERSCORE '_'
#define COMMA ','
#define ZERO '0'

/* file permissions */
#define READ_FILE_PERMISSION "r"
#define WRITE_FILE_PERMISSION "w"

/* file extensions */
#define ASMB_FILE_EXTEN ".as"
#define ASMB_MCRO_FILE_EXTEN ".am"
#define OBJECT_FILE_EXTEN ".ob"
#define EXTERNAL_FILE_EXTEN ".ext"
#define ENTRIES_FILE_EXTEN ".ent"

/* args and operators indices */
#define FIRST_ARG_ID 1
#define SEC_ARG_ID 2
#define DEST_ONLY 1
#define SRC_AND_DEST 2

#define IC_DC_RESET_VALUE 0


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
    IMMEDIATE_ADDR,
    DIRECT_ADDR,
    REGISTER_ADDR,
    MATRIX_ADDR,
    ERROR
} LineArg;


/* forward declarations */
void decimal_to_base4(char *, char *);

long file_content_size(FILE*);
FILE* open_file(char *, char *);

/* Pre-Assembler */
LineArg detect_arg_type(char *);
char* get_macro_name(char *);
int is_macro_call(char *, char *);

/* First/Second Pass */
int is_operation(char *);
int is_instruction(char *);
int is_register(char *);
LineArg get_operand_type(char *);
unsigned int get_operand_code_from_type(LineArg);
void set_word_operand_field(LineArg, int, int, unsigned int *, unsigned int *);

#endif