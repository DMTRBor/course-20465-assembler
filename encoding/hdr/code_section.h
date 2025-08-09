#ifndef __CODE_SECTION_H__
#define __CODE_SECTION_H__


#include "../hdr/util.h"


/* Code */
int encode_op_name(MemoryUnit **, char *, unsigned int, unsigned int, int, int *);
int encode_operand_by_type(char *, int, unsigned int, int, int *, MemoryUnit **);
int encode_operands(MemoryUnit **, char *, unsigned int, unsigned int, int, int *, int);
int encode_instruction(char *, int, MemoryUnit **, int);

#endif