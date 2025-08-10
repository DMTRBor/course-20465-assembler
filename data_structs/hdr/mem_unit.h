#ifndef __MEM_UNIT_H__
#define __MEM_UNIT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../machine/hdr/machine.h"


/* memory map unit */
typedef struct MemoryUnit {
    char *label_op_name;  /* optional - to store label operand */
    unsigned int address;
    Word encoded_value;
    struct MemoryUnit *next;
} MemoryUnit;


/* forward declarations */
MemoryUnit* new_mem_unit(void);
void set_mem_unit_addresses(MemoryUnit **, unsigned int);
void set_word_fields(MemoryUnit *,
                     unsigned int, 
                     unsigned int,
                     unsigned int, 
                     unsigned int);
void add_mem_unit_to_table(MemoryUnit **, MemoryUnit *);
void free_mem_table(MemoryUnit *);
void add_unit_and_increment_L(MemoryUnit **, MemoryUnit *, int *);

#endif