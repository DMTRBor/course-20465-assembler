#ifndef __MEM_UNIT_H__
#define __MEM_UNIT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../machine/hdr/machine.h"


#define DEFAULT_MEM_UNIT_ADDR 0  /* default address */


/* memory map unit (node) */
typedef struct MemoryUnit {
    char *label_op_name;  /* optional - to store label operand */
    unsigned int address;  /* memory unit address */
    Word encoded_value;  /* encoded (bits segmentation) word value */
    struct MemoryUnit *next;  /* pointer to next memory unit */
} MemoryUnit;


/**
 * The function creates a new memory
 * unit, when it's fields are
 * initialized with default values.
 * It receives no arguments and returns
 * a pointer to a newly created memory
 * unit (node).
 */
MemoryUnit* new_mem_unit(void);


/**
 * The function receives:
 * - address of the memory units table head
 * - address (machine) of the first memory unit
 * It traverses the table and assigns for each memory
 * unit it's address by calculating the offset from
 * the first unit address, incrementing it each next
 * unit in table.
 * The function does not return any value.
 */
void set_mem_unit_addresses(MemoryUnit **, unsigned int);


/**
 * The function receives:
 * - pointer to the memory unit
 * - values of Word structure fields:
 *    - encoding type code
 *    - destination operand
 *    - source operand and
 *    - operation code
 * (as defined in machine architecture).
 * The values of the Word fields are set to each field
 * in Word that is a field in memory unit structure.
 * The function does not return any value.
 */
void set_word_fields(MemoryUnit *, unsigned int, unsigned int,
                     unsigned int, unsigned int);


/**
 * The function receives:
 * - address of the head of memory units table
 * - pointer to the memory unit (node)
 * It adds the memory unit at the the tail of
 * the table (list). If the table is empty,
 * memory unit assigned as the head of the table.
 * The function does not return any value.
 */
void add_mem_unit_to_table(MemoryUnit **, MemoryUnit *);


/**
 * The function receives pointer to the memory
 * units table (list) and frees the table by
 * deallocating all the memory allocated for the
 * memory units (linked list) including the fields
 * of each memory unit, if needed.
 * The function does not return any value.
 */
void free_mem_table(MemoryUnit *);


/**
 * The function receives:
 * - address of the head of memory units table
 * - address of the memory unit (node)
 * address of the L - a number of words the operation
 * occupies in machine memory, that is also the number
 * of memory units occupied for instruction in machine
 * code. The function adds the memory unit to table of
 * memory units and increments the L number, counting it.
 * The function does not return any value.
 */
void add_unit_and_increment_L(MemoryUnit **, MemoryUnit *, int *);

#endif