#ifndef __ERRORS_HANDLING_H__
#define __ERRORS_HANDLING_H__

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../hdr/machine.h"
#include "../hdr/tables.h"
#include "../hdr/utils.h"


#define OPERANDS_NUMBER_ERROR -1


int is_macro_name_valid(char *, int);
int is_macro_args_num_valid(char *, int);
int is_operation(char *);
int is_instruction(char *);
int is_valid_label(char *, int);
LineArg detect_and_validate_first_arg(char *, int);
int get_num_of_operands(char *, int);
int is_operands_legal(char *, int);
int is_operands_num_valid(char *, int);

#endif