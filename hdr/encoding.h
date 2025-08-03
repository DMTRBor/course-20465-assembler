#ifndef __ENCODING_H__
#define __ENCODING_H__

#include "../hdr/errors_handling.h"

#define FIRST_ARG 0
#define MAT_FIRST_INDEX 1
#define MAT_SECOND_INDEX 2

#define WORDS_NUM_ERROR -1

#define NULL_TERMINATOR_ASCII 0

/* Encode fields from a combined value */
#define UINT_BITS_TO_DEST(val)       ((val)        & 0x3)
#define UINT_BITS_TO_SRC(val)       (((val) >> 2)  & 0x3)
#define UINT_BITS_TO_OPCODE(val)    (((val) >> 4)  & 0xF)
#define UINT_TO_UPPER_4_BITS(val)    ((val)        & 0xF)

#define INT_BITS_TO_ENC_TYPE(val)       ((val)        & 0x3)
#define INT_BITS_TO_DEST(val)          (((val) >> 2)  & 0x3)
#define INT_BITS_TO_SRC(val)           (((val) >> 4)  & 0x3)
#define INT_BITS_TO_OPCODE(val)        (((val) >> 6)  & 0xF)

void encode_operand_8_bit(Word *, int);
void encode_data_10_bit(Word *, int);
void encode_operand_first_index(Word *, unsigned int);
void encode_operand_sec_index(Word *, unsigned int);
int encode_matrix_row_col(Word *, char *, int);
int encode_both_registers(MemoryUnit **, char *, int, int *);
int encode_directive_number(MemoryUnit **, int, int, int *);

/* Code */
int encode_op_name(MemoryUnit **, char *, unsigned int, unsigned int, int, int *);
int encode_operand_by_type(char *, int, unsigned int, int, int *, MemoryUnit **);
int encode_operands(MemoryUnit **, char *, unsigned int, unsigned int, int, int *, int);
int encode_instruction(char *, int, MemoryUnit **, int);

/* Data */
int encode_data_direc(char *, MemoryUnit **, int);
int encode_string_direc(char *, MemoryUnit **, int);
int encode_mat_direc(char *, MemoryUnit **, int);

#endif