#ifndef __ERRORS_HANDLING_H__
#define __ERRORS_HANDLING_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../hdr/machine.h"
#include "../hdr/tables.h"
#include "../hdr/utils.h"


#define OPERANDS_NUM_ERROR -1
#define WORDS_NUM_ERROR -1

#define FIRST_ARG 0

/* Pre-Assembler */
int is_macro_name_valid(char *, int);
int is_macro_args_num_valid(char *, int);

/* First/Second Pass */
int is_valid_label(char *, int);
int is_operands_legal(char *, int);
int is_operands_num_valid(char *, int);

LineArg detect_and_validate_first_arg(char *, int);
int get_num_of_operands(char *, int);

/* Encoding */
int encode_operation(MemoryUnit **, char *, unsigned int, unsigned int, int);
int encode_op_sentence(char *, int, MemoryUnit **, int);

#endif