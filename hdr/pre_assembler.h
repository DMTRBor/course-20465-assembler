#ifndef __PRE_ASSEMBLER__H__
#define __PRE_ASSEMBLER__H__

#include "../hdr/utils.h"
#include "../hdr/const_tables.h"
#include "../hdr/data_struct.h"
#include "../hdr/errors_handling.h"
#include "../hdr/first_pass.h"


#define MACRO_START "mcro"
#define MACRO_END "mcroend"
#define MAX_MACROS_ALLOWED 50

#define COMMENT_SIGN ";"

#define NUM_OF_MCRO_ARGS 2
#define NUM_OF_MCROEND_ARGS 1

typedef enum {
    EMPTY_LINE,
    COMMENT,
    MCRO,
    MCROEND,
    OTHER
} LineArg;


int parse_assembler_source(FILE *, char *);
LineArg parse_line(char *);
char* get_macro_name(char *);
int is_macro_call(char *, char *);
void free_list_and_macro(Line *, Macro *);

#endif