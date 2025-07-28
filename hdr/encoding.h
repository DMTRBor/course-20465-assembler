#ifndef __ENCODING_H__
#define __ENCODING_H__

#include "../hdr/errors_handling.h"

#define MAT_DELIMITERS " \t\r[]"

#define FIRST_ARG 0
#define MAT_FIRST_INDEX 1
#define MAT_SECOND_INDEX 2

#define WORDS_NUM_ERROR -1

/* Encode fields from a combined value */
#define UINT_BITS_TO_DEST(val)       ((val)        & 0x3)
#define UINT_BITS_TO_SRC(val)       (((val) >> 2)  & 0x3)
#define UINT_BITS_TO_OPCODE(val)    (((val) >> 4)  & 0xF)
#define UINT_TO_UPPER_4_BITS(val)    ((val)        & 0xF)

void encode_operand_8_bit(Word *, int);
void encode_operand_first_index(Word *, unsigned int);
void encode_operand_sec_index(Word *, unsigned int);
int encode_matrix_row_col(Word *, char *, int);
int encode_both_registers(MemoryUnit **, char *, int, int *);

int encode_operation(MemoryUnit **, char *, unsigned int, unsigned int, int, int *);
int encode_operand_by_type(char *, int, unsigned int, int, int *, MemoryUnit **);
int encode_operands(MemoryUnit **, char *, unsigned int, unsigned int, int, int *, int);
int encode_instruction(char *, int, MemoryUnit **, int);

#endif