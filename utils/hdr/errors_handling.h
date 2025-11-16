#ifndef __ERRORS_HANDLING_H__
#define __ERRORS_HANDLING_H__

#include "../../machine/hdr/machine.h"
#include "../../utils/hdr/utils.h"


#define COMMA_CHAR ','
#define WHITESPACE_CHAR ' '
#define TAB_CHAR '\t'


/* Pre-Assembler */

/**
 * The function receives:
 * - line string with macro definition
 *   and name
 * - line number
 * The function checks if the macro
 * definition line "mcro <name>",
 * does not have an invalid macro name,
 * i.e has a name of operation, directive
 * or register.
 * If the macro name contains one of the
 * above, the function will return FALSE
 * code as it's an illegal name.
 * Otherwise, TRUE code will be returned.
 */
int is_macro_name_valid(char *, int);


/**
 * The function receives:
 * - line string with macro definition
 *   or closure statement (mcro, mcroend)
 * - expected number of arguments in
 *   macro definition/closure line
 * The function count the number of
 * arguments in a definition/closure line
 * (that have a constant number of arguments
 * that should be in this line) and checks
 * if the number of arguments this line
 * contains is correct.
 * If the number of counted arguments is
 * not equal to expected, returned FALSE.
 * Otherwise, TRUE code will be returned.
 */
int is_macro_args_num_valid(char *, int);


/* First/Second Pass */

/**
 * The function receives:
 * - label argument string/token
 * - line number
 * The function checks if the label name
 * is valid according to requirements:
 * - starts with a letter
 * - contains alpha-numeric characters only
 * If the label is not defined according to
 * the requirements above, returned FALSE.
 * Otherwise, TRUE code will be returned.
 */
int is_valid_label(char *, int);


/**
 * The function receives a string
 * data that appears in string
 * directive and should be encoded,
 * and validates it has a proper
 * quotation marks - contains only
 * 2 and could be encoded.
 * If the string data (token) contains
 * more/less than 2 quotation marks, the
 * function will return FALSE code, as
 * it's an invalid string data that could
 * not be encoded.
 * Otherwise, a TRUE code will be returned.
 */
int is_valid_string_data(char *);


/**
 * The function receives:
 * - matrix directive line string
 * - line number
 * The function checks if the directive
 * with matrix definition contains sub-
 * sequent or trailing commas - that is
 * an error in arguments passing and
 * could affect further machine code
 * creation.
 * If no trailing or subsequent commas
 * found in directive arguments, TRUE code
 * will be returned. Otherwise, FALSE code
 * will be returned.
 */
int has_mat_direc_subseq_commas(char *, int);


/**
 * The function receives:
 * - a string that should contain a
 *   numeric operand
 * - line number
 * The function checks if the operand
 * could be converted from a numeric
 * string representation to a valid
 * integer number that could be stored
 * in machine memory.
 * If the string could not be converted
 * to integer or the converted number is
 * within an invalid range (smaller or
 * bigger than memory could store), a
 * FALSE code will be returned.
 * Otherwise, a TRUE code will be returned.
 */
int is_operand_value_legal(char *, int);


/**
 * The function receives:
 * - line which contains instruction
 *   (operation + operands)
 * - a valid number of operands for
 *   this operation
 * The function checks if the number
 * of operands for the defined operation
 * in this line is correct and if so, a
 * TRUE code will be returned.
 * If the operation is not found or an
 * invalid number of operands is found,
 * a FALSE code will be returned.
 */
int is_operands_num_valid(char *, int);


/**
 * The function receives:
 * - source operand code
 * - opcode index in opcodes table
 * The function checks if the addressing
 * method for the source operand for
 * operation whose index is given as a
 * parameter is legal.
 * If the addressing method is legal, a
 * TRUE code will be returned.
 * Otherwise, FALSE code will be returned.
 */
int is_src_addr_method_valid(unsigned int, int);


/**
 * The function receives:
 * - destination operand code
 * - opcode index in opcodes table
 * The function checks if the addressing
 * method for the destination operand for
 * operation whose index is given as a
 * parameter is legal.
 * If the addressing method is legal, a
 * TRUE code will be returned.
 * Otherwise, FALSE code will be returned.
 */
int is_dest_addr_method_valid(unsigned int, int);


/**
 * The function receives:
 * - operation name
 * - destination operand code
 * - source operand code
 * - number of operands in this
 *   instruction
 * The function checks if the addressing
 * methods for this operation source or
 * destination or both (depends on operation
 * and the context of usage of this
 * operation) are legal.
 * If the addressing method/s is/are legal,
 * a TRUE code will be returned.
 * Otherwise, FALSE code will be returned.
 */
int is_legal_addressing_methods(char *, unsigned int, unsigned int, int);


/**
 * The function receives:
 * - line string from source/macro
 *   parsed file
 * - line number
 * The function detects the type of
 * the first argument appearing in
 * the line given as parameter.
 * The types defined by code enumerations
 * of LineArg enumeration.
 * The argument type is returned while
 * the default type is ERROR (in case
 * nothing has been detected of other
 * type).
 */
LineArg detect_and_validate_first_arg(char *, int);

#endif