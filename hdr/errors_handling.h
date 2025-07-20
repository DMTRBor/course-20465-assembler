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
#define FIRST_ARG_ID 1
#define SEC_ARG_ID 2
#define DEST_ONLY 1
#define SRC_AND_DEST 2

/* Pre-Assembler */
int is_macro_name_valid(char *, int);
int is_macro_args_num_valid(char *, int);

/* First/Second Pass */
int is_operation(char *);
int is_instruction(char *);
int is_valid_label(char *, int);
int is_operands_legal(char *, int);
int is_operands_num_valid(char *, int);

LineArg detect_and_validate_first_arg(char *, int);
int get_num_of_operands(char *, int);
LineArg get_operand_type(char *);
unsigned int get_operand_code_from_type(LineArg);
void set_word_operand_field(LineArg, int, int, unsigned int *, unsigned int *);

/* Encoding */
int encode_operation(MemoryUnit *, char *, unsigned int, unsigned int, int);
int encode_op_sentence(char *, int, MemoryUnit *, int);

#endif