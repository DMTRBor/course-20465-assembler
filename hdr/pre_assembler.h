#ifndef __PRE_ASSEMBLER__H__
#define __PRE_ASSEMBLER__H__

#include "../hdr/utils.h"
#include "../hdr/commands.h"
#include "../hdr/data_struct.h"


#define MACRO_START "mcro"
#define MACRO_END "mcroend"

#define WHITESPACE " \t"
#define COMMENT_SIGN ";"

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

#endif