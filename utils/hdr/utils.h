#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "../../machine/hdr/machine.h"


/* one last character is null terminator */
#define MAX_FNAME_LEN 51  /* maximum filename length */
#define MAX_LINE_LEN 81  /* maximum line length in file */

#define MIN_ALLOWED_ARGS 2  /* minimum allowed command line arguments */
#define NUM_OF_EXTERN_ARGS 1  /* number of arguments for extern directive */
#define VALID_NUM_OF_QUOT_MARKS 2  /* correct number of quotation marks */

#define BASE_10 10

#define STR_EQUAL 0  /* string equality code */

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
#define MIN_MAT_SIZE 1  /* mininum valid size for matrix - rows x columns */
#define MAX_MAT_SIZE 15  /* maximum valid size for matrix - rows x columns */
#define ILLEGAL_MAT_SIZE 0  /* illegal matrix size code */


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

/* line argument type enumeration */
typedef enum {
    EMPTY_LINE,
    COMMENT,
    MCRO,
    MCROEND,
    LABEL,
    INSTRUCTION,
    DIRECTIVE,
    IMMEDIATE_ADDR,  /* immediate addressing */
    DIRECT_ADDR,     /* direct addressing */
    REGISTER_ADDR,   /* register addressing */
    MATRIX_ADDR,     /* matrix addressing */
    ERROR
} LineArg;


/**
 * This function receives a pointer to
 * a string and returns a new dynamically
 * allocated string that is a duplicate
 * of the input string.
 */
char* duplicate_str(const char *);


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

/**
 * The function receives a string with
 * line content, pointing to a start of
 * and arguments. It parses the string
 * and detects the type of the first
 * argument it points to. If the first
 * argument (token) is not recognized as
 * one that belongs to LineArg enumeration
 * it will continue to next token and try
 * to detect in the next token.
 * The argument type is returned while
 * the default type is ERROR (in case
 * nothing has been detected of other type).
 */
LineArg detect_arg_type(char *);


/**
 * The function receives a string
 * with line that contains a macro
 * definition, tokenizes it and
 * gets the macro name from macro
 * definition line.
 * The function returns a string
 * that contains the macro name
 * only (name validation done
 * before this, using another function).
 */
char* get_macro_name(char *);


/**
 * The function receives:
 * - string with line that possibly
 *   contains a macro call.
 * - a macro name
 * The function checks if a macro
 * call appears in given line and
 * returns TRUE if there is a macro
 * call in this line, and FALSE code
 * otherwise.
 */
int is_macro_call(char *, char *);

/* First/Second Pass */

/**
 * The function receives a string
 * which possibly is an operation
 * name (part of instruction) and
 * checks if the given string is
 * a legal operation name. If this 
 * string is an operation name,
 * TRUE code is returned. Otherwise,
 * FALSE code is returned.
 */
int is_operation(char *);


/**
 * The function receives a string
 * which possibly is a directive
 * name and checks if the given
 * string is a legal directive name.
 * If this string is a directive
 * name, TRUE code is returned.
 * Otherwise, FALSE code is returned.
 */
int is_directive(char *);


/**
 * The function receives a string
 * which possibly is a register
 * name and checks if the given
 * string is a legal register name.
 * If this string is a register
 * name, TRUE code is returned.
 * Otherwise, FALSE code is returned.
 */
int is_register(char *);


/**
 * The function receives:
 * - string (line argument) that
 *   should contain a directive
 * - an expected directive that
 *   should be in this string
 * The function compares the string
 * that is passed as parameter to the
 * expected string and returns code
 * TRUE if the expected string and the
 * give line argument are equal and
 * FALSE code otherwise.
 */
int is_expected_directive(char *, char *);


/**
 * The function receives a string with
 * label argument and checks if the
 * label name is a reserved name for
 * machine OS - a register name,
 * operation name or a directive name.
 * A label name could not be the same
 * as one of the reserved names in the
 * machine OS.
 * The function returns TRUE code if the
 * argument is one of the: register,
 * operation or directive names.
 * Otherwise, FALSE code returned.
 */
int is_label_reserved_name(char *);


/**
 * The function receives a line
 * content string, tokenizes it
 * using defined delimitesrs to 
 * parse the arguments, and counts
 * the number of arguments in line
 * without operation/directive name.
 * The function returns number of
 * arguments - operands or directive
 * arguments without operation/directive
 * name.
 * If the line is empty, zero will be
 * returned.
 */
int get_num_of_arguments(char *);


/**
 * The function receives a string
 * that contains an operand in an
 * instruction and determines the
 * operand type that will be mapped
 * to corresponding addressing method
 * in another function.
 * The operand (arg) type is returned while
 * the default type is ERROR (in case
 * nothing has been detected of other type).
 */
LineArg get_operand_type(char *);


/**
 * The function receives an enumeration
 * (LineArg) that represents an operand
 * type and maps it to addressing method
 * code that is returned from this function.
 * There are 4 addressing methods:
 * - immediate (0)
 * - direct (1)
 * - matrix (2)
 * - register (3)
 * The default addressing method is immediate
 * which is represented by code 0, that is also
 * the default (empty) value for a field in word
 * in machine memory.
 */
AddrMethodCode get_operand_code_from_type(LineArg);


/**
 * The function receives:
 * - operand type enumeration (LineArg)
 * - argument index
 * - number of operands
 * - address of destination operand
 *   addressing method code
 * - address of source operand
 *   addressing method code
 * The function determines the addressing
 * method of the operand by type of operand -
 * source/destination, and sets the source/
 * destination field in word structure to
 * the correct addressing method code.
 * The function does not return any value.
 */
void set_word_operand_field(LineArg, int, int, unsigned int *, unsigned int *);


/**
 * The function receives an
 * address of line in lines list.
 * It extracts the label name and
 * the function returns the label
 * name from the line and sets the
 * pointer of the line after the
 * label, skipping all the
 * whitespaces and placing it at
 * the beginning of the first
 * argument that goes after the label.
 */
char* get_label_name(char **);


/**
 * The function receives:
 * - string representation of an
 *   integer number
 * - address of an integer number
 * - line number
 * The function converts the string
 * to integer number and stores it
 * in variable that is passed as a
 * parameter.
 * If the conversion fails, a code
 * STATUS_CODE_ERR will be returned.
 * Otherwise, STATUS_CODE_OK will
 * be returned.
 */
int str_to_int(char *, int *, int);


/**
 * The function receives the
 * matrix dimensions from .mat
 * directive arguments, in format:
 * - [row][column]
 * parses the row and the column
 * values and calculates the total
 * size of the matrix in memory
 * units, required to allocation.
 * If the matrix size is within the
 * allowed range, it will return the
 * total matrix size in memory units.
 * Otherwise, illegal code indicator
 * will be reurned to signal there is
 * a problem with matrix dimensions
 * definition.
 */
int get_mat_size(char *);


/**
 * The function receives:
 * - a string that contains a matrix
 *   name
 * - an address of field in memory
 *   unit structure which stores a
 *   label name
 * The function parses the matrix name
 * from line string that contains it
 * and stores the matrix name, as it
 * is a label, in label field in memory
 * unit structure. This is reserved for
 * second pass processing to indicate
 * a memory unit that is for label
 * encoding.
 * The function does not return any value.
 */
void store_matrix_name(char *, char **);

#endif