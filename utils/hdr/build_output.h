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


/**
 * This function receives an unsigned integer
 * decimal number and an empty string, converts the
 * decimal number to base 4 representation
 * and places the converted string representation of
 * the number in base 4 in an empty string.
 */
void decimal_to_base4(unsigned int, char *);

/**
 * This function receives a Word (10-bit) unsigned
 * integer and a pointer to a string to store the
 * converted value and converts it to base 4
 * representation string.
 */
void word_to_base4(Word, char *);

int code_and_data_size_base4_to_file(unsigned int, unsigned int, char *, char *);

int write_full_content_to_file(char *, char *, char *);

int build_object_file(char *, unsigned int, unsigned int, MemoryUnit **);
int build_ext_file(char *, MemoryUnit **);
int build_ent_file(char *, Label **);

#endif