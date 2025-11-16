#ifndef __CODE_SECTION_H__
#define __CODE_SECTION_H__


#include "../hdr/util.h"


/**
 * The function receives:
 * - address of the head of memory units table
 * - operation name
 * - destination operand addressing method code
 * - source operand addressing method code
 * - line number
 * - address of L number - the number of total
 *   words/memory units for instruction
 * The function encodes the operation name, while
 * the encoded word requires also the addressing
 * methods of the operands. A new memory
 * unit is created and the fields of the encoded
 * word are set. After that, memory unit is added
 * to the table and L number is incremented.
 * The function returns STATUS_CODE_OK if the encoding
 * was successful and STATUS_CODE_ERR otherwise.
 */
int encode_op_name(MemoryUnit **, char *, unsigned int, unsigned int, int, int *);


/**
 * The function receives:
 * - operand name
 * - flag that indicates if the operand is a source
 *   operand
 * - operand addressing method code
 * - line number
 * - address of L number - the number of total
 *   words/memory units instruction
 * - address of the head of memory units table
 * It encodes the operand by it's addressing method
 * type and it's type - source or destination.
 * After that, memory unit is added to the
 * table and L number is incremented. The function returns
 * STATUS_CODE_OK if the encoding was successful and
 * STATUS_CODE_ERR otherwise.
 */
int encode_operand_by_type(char *, int, unsigned int, int, int *, MemoryUnit **);


/**
 * The function receives:
 * - address of the head of memory units table
 * - operation name
 * - destination operand addressing method code
 * - source operand addressing method code
 * - line nummber
 * - address of L number - the number of total
 *   words/memory units for instruction
 * - total number of operands
 * It parses the operands, detects it's type and
 * addressing method and encodes it using the utility
 * encode_operand_by_type. The function returns
 * STATUS_CODE_OK if the encoding was successful and
 * STATUS_CODE_ERR otherwise.
 */
int encode_operands(MemoryUnit **, char *, unsigned int, unsigned int, int, int *, int);


/**
 * This is the function that encodes the whole
 * instruction line, using the above utilities
 * and other helpers.
 * It receives:
 * - line that contains the instruction
 *   (operation + operands)
 * - number of operands in this line
 * - address of the head of memory units table
 * - line number
 * It parses the line with the instruction
 * encodes the operation and the operands and other
 * needed to encode things and stores the newly
 * created memory units for this instruction in
 * memory table, while also counting the L number -
 * the number of total words/memory units for
 * instruction.
 * The function returns the number of total words/
 * memory units used in encoding process if the
 * encoding went well, without errors. Otherwise,
 * WORDS_NUM_ERROR (error code) is returned.
 */
int encode_instruction(char *, int, MemoryUnit **, int);

#endif