#ifndef __UTIL_H__
#define __UTIL_H__

#include "../../utils/hdr/errors_handling.h"
#include "../../data_structs/hdr/mem_unit.h"
#include "../../data_structs/hdr/label.h"


#define FIRST_ARG 0         /* first argument index constant */
#define MAT_FIRST_INDEX 1   /* matrix first argument index constant */
#define MAT_SECOND_INDEX 2  /* matrix second argument index constant */

#define WORDS_NUM_ERROR -1  /* words count error code */

#define NULL_TERMINATOR_ASCII 0  /* ASCII code of null terminator */


/* helper macro functions */
/* write to fields from a combined value, ensure portability */
#define UINT_BITS_TO_DEST(val)       ((val)        & 0x3)
#define UINT_BITS_TO_SRC(val)       (((val) >> 2)  & 0x3)
#define UINT_BITS_TO_OPCODE(val)    (((val) >> 4)  & 0xF)
#define UINT_TO_UPPER_4_BITS(val)    ((val)        & 0xF)

#define INT_BITS_TO_ENC_TYPE(val)     UINT_BITS_TO_DEST(val)  /* use implemented, same bits */
#define INT_BITS_TO_DEST(val)          UINT_BITS_TO_SRC(val)  /* use implemented, same bits */
#define INT_BITS_TO_SRC(val)           (((val) >> 4)  & 0x3)
#define INT_BITS_TO_OPCODE(val)        (((val) >> 6)  & 0xF)


/**
 * The function receives:
 * - pointer to a word in machine memory
 * - integer with 8-bit value
 * The function writes to the fields of
 * the word using masking and bit shifts
 * to "split" the 8-bit value in a way so
 * each set of bits in a bitfield in word
 * will get the required value, assuring
 * the 8-bit value will be written to
 * these bits and not to other.
 * The function does not return any value.
 */
void encode_operand_8_bit(Word *, int);


/**
 * The function receives:
 * - pointer to a word (10-bit unsigned
 *   integer) in machine memory
 * - integer with 10-bit value
 * The function writes to the fields of
 * the word using masking and bit shifts
 * to "split" the 10-bit value in a way so
 * each set of bits in a bitfield in word
 * will get the required value, assuring
 * the 10-bit value will be written to
 * these bits and not to other.
 * The function does not return any value.
 */
void encode_data_10_bit(Word *, int);


/**
 * The function receives:
 * - pointer to a word (10-bit unsigned
 *   integer) in machine memory
 * - register index (extracted from r<X>)
 * The function encodes the index of the
 * register in the upper 4 bits of the
 * 10-bit word, i.e bits 6-9, that are
 * occupied by opcode field in word
 * structure. In matrix, this represents
 * the row index.
 * The function does not return any value.
 */
void encode_operand_first_index(Word *, unsigned int);


/**
 * The function receives:
 * - pointer to a word (10-bit unsigned
 *   integer) in machine memory
 * - register index (extracted from r<X>)
 * The function encodes the index of the
 * register in the 4 bits of the 10-bit
 * word, that come after the upper 4 bits
 * i.e bits 2-5, that are occupied by source
 * and the destination operands fields in
 * word structure. In matrix, this represents
 * the column index.
 * The function does not return any value.
 */
void encode_operand_sec_index(Word *, unsigned int);


/**
 * The function receives:
 * - pointer to a word (10-bit unsigned
 *   integer) in machine memory
 * - a line that contains the operand to
 *   encode, and will be extracted
 * - line number
 * The function encodes the row and the
 * column indices in a word structure -
 * 4 upper bits for row index and 4 next
 * bits for column index.
 * If errors occured during encoding, will
 * be returned error code STATUS_CODE_ERR.
 * Otherwise, STATUS_CODE_OK will be returned.
 */
int encode_matrix_row_col(Word *, char *, int);


/**
 * The function receives:
 * - address of the memory units table head
 * - argument string
 * - line number
 * - address of L number (number of words)
 * The function encodes the operands while
 * both operands were detected as registers.
 * In this case, each register index will be
 * encoded in 4 bits (upper and the following)
 * while source operand will take the upper 4
 * bits and the destination the next 4 bits.
 * There will be only a single word required
 * for encoding.
 * The newly created memory unit is added to
 * memory map and L counted.
 * If errors occured during encoding, will
 * be returned error code STATUS_CODE_ERR.
 * Otherwise, STATUS_CODE_OK will be returned.
 */
int encode_both_registers(MemoryUnit **, char *, int, int *);


/**
 * The function receives:
 * - address of the memory units table head
 * - integer number to encode
 * - line number
 * - address of L number (number of words)
 * The function encodes the number that
 * is coming from a directive using 
 * the whole word space - 10 bits and 2's
 * complement arithmetic (for negatives).
 * The newly created memory unit is added to
 * memory map and L counted.
 * If errors occured during encoding, will
 * be returned error code STATUS_CODE_ERR.
 * Otherwise, STATUS_CODE_OK will be returned.
 */
int encode_directive_number(MemoryUnit **, int, int, int *);


/**
 * The function receives:
 * - address of memory map (table)
 * - address of labels map (table)
 * The function scans the memory map and checks
 * for labels whose addresses were not encoded
 * in first pass because then their addresses
 * were unknown. When the label is found, it's
 * address is fetched from lables table and
 * encoded at memory unit field that holds the
 * address in memory of the label used in this
 * instruction.
 * This completes the machine code before
 * writing it to separate files.
 * If the label is not found in labels table,
 * will be returned error code STATUS_CODE_ERR.
 * Otherwise, STATUS_CODE_OK will be returned.
 */
int encode_labels_addresses(MemoryUnit **, Label **);

#endif