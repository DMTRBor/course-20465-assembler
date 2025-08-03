#ifndef __ERRORS_HANDLING_H__
#define __ERRORS_HANDLING_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../hdr/machine.h"
#include "../hdr/tables.h"
#include "../hdr/utils.h"

/* Pre-Assembler */
int is_macro_name_valid(char *, int);
int is_macro_args_num_valid(char *, int);

/* First/Second Pass */
int is_valid_label(char *, int);
int is_operand_value_legal(char *, int);
int is_operands_num_valid(char *, int);
int is_src_addr_method_valid(unsigned int, int);
int is_dest_addr_method_valid(unsigned int, int);
int is_legal_addressing_methods(char *, unsigned int, unsigned int, int);

LineArg detect_and_validate_first_arg(char *, int);

#endif