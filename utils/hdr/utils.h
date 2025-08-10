#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "../../machine/hdr/machine.h"

/* one last character is null terminator */
#define MAX_FNAME_LEN 51
#define MAX_LINE_LEN 81

#define MIN_ALLOWED_ARGS 2
#define NUM_OF_EXTERN_ARGS 1

#define BASE_10 10

#define STR_EQUAL 0

/* delimiters and hallmarks */
#define MACRO_START "mcro"
#define MACRO_END "mcroend"
#define LABEL_END_SIGN ':'
#define QUOTATION_MARK '"'
#define IMMEDIATE_ADDR_SIGN '#'
#define MAT_LEFT_BRACE '['
#define MAT_RIGHT_BRACE ']'
#define COMMENT_SIGN ';'
#define NEWLINE_STR "\n"

#define WHITESPACE " \t"
#define OP_DELIMITERS " \t\r,"
#define MAT_DELIMITERS " \t\r[]"

#define NEWLINE_CHAR '\n'
#define NULL_TERMINATOR '\0'
#define UNDERSCORE '_'

/* file permissions */
#define READ_FILE_PERMISSION "r"
#define WRITE_FILE_PERMISSION "w"

/* minimum possible number of
   characters written to file */
#define MIN_CHARS_WRITTEN 0

/* file extensions */
#define ASMB_FILE_EXTEN ".as"
#define ASMB_MACRO_FILE_EXTEN ".am"
#define OBJECT_FILE_EXTEN ".ob"
#define EXTERNAL_FILE_EXTEN ".ext"
#define ENTRIES_FILE_EXTEN ".ent"

/* args and operators indices */
#define FIRST_ARG_ID 1
#define SEC_ARG_ID 2

/* matrix encoding */
#define MIN_MAT_SIZE 1
#define MAX_MAT_SIZE 15
#define ILLEGAL_MAT_SIZE 0


/* status codes */
enum {
    STATUS_CODE_OK,
    STATUS_CODE_ERR
};

/* general true/false */
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
    INSTRUCTION,
    DIRECTIVE,
    IMMEDIATE_ADDR,
    DIRECT_ADDR,
    REGISTER_ADDR,
    MATRIX_ADDR,
    ERROR
} LineArg;

/**
 * This function receives a data
 * stream read from a file and
 * returns the size of the stream/
 * content in bytes.
 */
long file_content_size(FILE*);

/**
 * This function receives a string
 * that represents a filename, opens
 * the file for reading and returns a
 * pointer to file if succeeded in
 * reading the content. If failed to
 * read from file (not exitent, permission),
 * the function returns null.
 */
FILE* open_file(char *, char *);

/* Pre-Assembler */
LineArg detect_arg_type(char *);
char* get_macro_name(char *);
int is_macro_call(char *, char *);

/* First/Second Pass */
int is_operation(char *);
int is_directive(char *);
int is_register(char *);
int is_expected_directive(char *, char *);

int get_num_of_arguments(char *);
LineArg get_operand_type(char *);
AddrMethodCode get_operand_code_from_type(LineArg);

void set_word_operand_field(LineArg, int, int, unsigned int *, unsigned int *);

char* get_label_name(char **);

int to_int(char *, int *, int);

int get_mat_size(char *);

void store_matrix_name(char *, char **);

#endif