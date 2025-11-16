#ifndef __BUILD_OUTPUT_H__
#define __BUILD_OUTPUT_H__


#include "../../utils/hdr/utils.h"
#include "../../data_structs/hdr/mem_unit.h"
#include "../../data_structs/hdr/label.h"


/* max for 255 max value */
#define MAX_DIGITS_BASE_4 5
#define BASE_4 4

#define ZERO '0'

/* file buffer max characters (already includes null terminator) */
#define MAX_FILE_BUFFER (MAX_LINE_LEN * MEMORY_SIZE)

/* word bit field masks */
#define OPCODE_MASK        0x0F    /* 4-bit mask - opcode */
#define OPERAND_MASK       0x03    /* 2-bit mask - operands */
#define ENC_TYPE_MASK      0x03    /* 2-bit mask - encoding type */
#define WORD_10_BIT_MASK   0x3FF   /* 10-bit mask */

/* bit positions for Word fields */
#define OPCODE_SHIFT        6       /* opcode bits 6-9 */
#define SRC_OPERAND_SHIFT   4       /* src_operand bits 4-5 */
#define DEST_OPERAND_SHIFT  2       /* dest_operand bits 2-3 */
#define ENC_TYPE_SHIFT      0       /* encoding_type bits 0-1 */

/* combining Word fields into a 10-bit value */
#define COMBINE_OPCODE(opcode)           (((opcode) & OPCODE_MASK) << OPCODE_SHIFT)
#define COMBINE_SRC_OPERAND(src)         (((src) & OPERAND_MASK) << SRC_OPERAND_SHIFT)
#define COMBINE_DEST_OPERAND(dest)       (((dest) & OPERAND_MASK) << DEST_OPERAND_SHIFT)
#define COMBINE_ENC_TYPE(encoding)       (((encoding) & ENC_TYPE_MASK) << ENC_TYPE_SHIFT)

/* Word to decimal conversion macro, ensures portability */
#define WORD_TO_DECIMAL(word) \
    ((COMBINE_OPCODE((word).opcode) | \
      COMBINE_SRC_OPERAND((word).src_operand) | \
      COMBINE_DEST_OPERAND((word).dest_operand) | \
      COMBINE_ENC_TYPE((word).encoding_type)) & WORD_10_BIT_MASK)


/**
 * This function receives:
 * - unsigned integer decimal number
 * - empty string
 * It converts the decimal number to base 4
 * representation and places the converted
 * string representation of the number in
 * base 4 in an empty string.
 * The function does not return any value.
 */
void decimal_to_base4(unsigned int, char *);


/**
 * This function receives:
 * - pointer to a word (10-bit unsigned
 *   integer) in machine memory
 * - string to store the converted
 *   to base 4 value
 * It converts the word to base 4
 * representation string and stores the
 * result in the string received as
 * parameter.
 * The function does not return any value.
 */
void word_to_base4(Word, char *);


/**
 * The function receives:
 * - ICF (instruction counter final) value
 * - DCF (instruction counter final) value
 * - buffer for machine code line
 * - buffer for machine code full content
 * The function converts the ICF and DCF to base 4
 * values, writes the converted values to line if
 * format "ICF  DCF" and adds the line to machine
 * code full content buffer.
 * If errors occured, will be returned error code
 * STATUS_CODE_ERR.
 * Otherwise, STATUS_CODE_OK will be returned.
 */
int code_and_data_size_base4_to_file(unsigned int, unsigned int, char *, char *);


/**
 * The function receives:
 * - file name string
 * - full encoded content buffer
 * - file extension
 * The function receives a buffer that contains
 * the encoded content and writes the buffer to
 * a file, then saves the file with given
 * extension.
 * If errors occured, will be returned error code
 * STATUS_CODE_ERR.
 * Otherwise, STATUS_CODE_OK will be returned.
 */
int write_full_content_to_file(char *, char *, char *);


/**
 * The function receives:
 * - object file name
 * - ICF (instruction counter final) value
 * - DCF (instruction counter final) value
 * - address of memory map (table)
 * The function builds the object file - the
 * file that contains ICF, DCF and the code
 * and data sections addresses and encoded values
 * in separated columns:
 * -         ICF             DCF
 *      base4_address   base4_encoded
 *              code section
 *              ------------
 *              data section
 * The buffer that contains all the machine code
 * is written to file with given name and
 * extension, creating object file.
 * If errors occured, will be returned error
 * code STATUS_CODE_ERR.
 * Otherwise, STATUS_CODE_OK will be returned.
 */
int build_object_file(char *, unsigned int, unsigned int, MemoryUnit **);


/**
 * The function receives:
 * - externals file name
 * - address of memory map (table)
 * The function builds the external labels
 * file - labels that have '.extern' directive:
 * -    label_name    label_address_base4
 * If some label mentinoned several times,
 * there would be a line for same label with
 * an address in memory where it's called.
 * The buffer that contains all the external
 * labels names in one column and the encoded
 * addresses in another is written to file
 * with given name and .ext extension,
 * creating externals file.
 * File with .ext extension is created only if
 * external directives included in assembler
 * source file.
 * If errors occured, will be returned error
 * code STATUS_CODE_ERR.
 * Otherwise, STATUS_CODE_OK will be returned.
 */
int build_ext_file(char *, MemoryUnit **);


/**
 * The function receives:
 * - entries file name
 * - address of the head of labels table
 * The function builds the entries labels
 * file - labels that have '.entry' directive:
 * -    label_name    label_address_base4
 * The addresses will be fetched from labels
 * table, after they will be detected by
 * type ENTRY.
 * The buffer that contains all the entries
 * labels names in one column and the encoded
 * addresses in another is written to file
 * with given name and .ent extension,
 * creating externals file.
 * File with .ent extension is created only if
 * entry directives included in assembler
 * source file.
 * If errors occured, will be returned error
 * code STATUS_CODE_ERR.
 * Otherwise, STATUS_CODE_OK will be returned.
 */
int build_ent_file(char *, Label **);

#endif