#ifndef __UTILS__H__
#define __UTILS__H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* one last character is null terminator */
#define MAX_FNAME_LEN 51
#define MAX_LINE_LEN 81

#define MIN_ALLOWED_ARGS 2

#define BASE_4 4
#define BASE_10 10

#define NEWLINE_STR "\n"
#define NEWLINE_CHAR '\n'
#define NULL_TERMINATOR '\0'
#define ZERO '0'

#define STR_EQUAL 0

#define READ_FILE_PERMISSION "r"
#define WRITE_FILE_PERMISSION "w"

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

/* forward declarations */
void decimal_to_base4(char *, char *);
long file_content_size(FILE*);
FILE* open_file(char *, char *);
int line_too_long(char *, FILE *);

#endif