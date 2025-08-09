#ifndef __UTIL_H__
#define __UTIL_H__

#include "../../utils/hdr/errors_handling.h"
#include "../../data_structs/hdr/mem_unit.h"
#include "../../data_structs/hdr/label.h"

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

#define INT_BITS_TO_ENC_TYPE(val)     UINT_BITS_TO_DEST(val)
#define INT_BITS_TO_DEST(val)          UINT_BITS_TO_SRC(val)
#define INT_BITS_TO_SRC(val)           (((val) >> 4)  & 0x3)
#define INT_BITS_TO_OPCODE(val)        (((val) >> 6)  & 0xF)

void encode_operand_8_bit(Word *, int);
void encode_data_10_bit(Word *, int);
void encode_operand_first_index(Word *, unsigned int);
void encode_operand_sec_index(Word *, unsigned int);
int encode_matrix_row_col(Word *, char *, int);
int encode_both_registers(MemoryUnit **, char *, int, int *);
int encode_directive_number(MemoryUnit **, int, int, int *);
int encode_labels_addresses(MemoryUnit **, Label **);

#endif