#ifndef __DATA_SECTION_H__
#define __DATA_SECTION_H__


#include "../hdr/util.h"


/**
 * The function receives:
 * - line which contains the data directive
 *   with all arguments
 * - address of the head of memory units table
 * - line number
 * It parses the line and encodes
 * the data directive with all the arguments.
 * The function returns the number of total words/
 * memory units used in encoding process if the
 * encoding went well, without errors. Otherwise,
 * WORDS_NUM_ERROR (error code) is returned.
 */
int encode_data_direc(char *, MemoryUnit **, int);


/**
 * The function receives:
 * - line which contains the data directive
 *   with all arguments
 * - address of the head of memory units table
 * - line number
 * It parses the line and encodes
 * the string directive and the string content
 * including a null terminator at the end of the
 * string.
 * The function returns the number of total words/
 * memory units used in encoding process if the
 * encoding went well, without errors. Otherwise,
 * WORDS_NUM_ERROR (error code) is returned.
 */
int encode_string_direc(char *, MemoryUnit **, int);


/**
 * The function receives:
 * - line which contains the data directive
 *   with all arguments
 * - address of the head of memory
 *   units table
 * - line number
 * It parses the line and encodes
 * the matrix directive with all the arguments.
 * The function returns the number of total words/
 * memory units used in encoding process if the
 * encoding went well, without errors. Otherwise,
 * WORDS_NUM_ERROR (error code) is returned.
 */
int encode_mat_direc(char *, MemoryUnit **, int);

#endif