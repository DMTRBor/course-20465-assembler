#include "../hdr/mem_unit.h"


MemoryUnit* new_mem_unit(void) {
    MemoryUnit *new_unit = (MemoryUnit*)malloc(sizeof(MemoryUnit));
    /* check if allocated properly */
    if (new_unit == NULL)
        return NULL;
    
    /* initialize fields with default values */
    new_unit->label_op_name = NULL;
    new_unit->address = 0;
    memset(&new_unit->encoded_value, 0, sizeof(Word));  /* reset word fields */
    new_unit->next = NULL;
    
    return new_unit;
}


void set_mem_unit_addresses(MemoryUnit **mem_map, unsigned int start_address) {
    MemoryUnit *unit = *mem_map;
    unsigned int unit_address = start_address;

    while (unit != NULL) {
        unit->address = unit_address++;
        unit = unit->next;
    }
}


void set_word_fields(MemoryUnit *unit,
                     unsigned int encoding_type, 
                     unsigned int dest_operand,
                     unsigned int src_operand, 
                     unsigned int opcode) {
    if (unit == NULL)
        return;
    
    unit->encoded_value.encoding_type = encoding_type;
    unit->encoded_value.dest_operand = dest_operand;
    unit->encoded_value.src_operand = src_operand;
    unit->encoded_value.opcode = opcode;
}


void add_mem_unit_to_table(MemoryUnit **mem_map, MemoryUnit *new) {
    MemoryUnit *unit;
    
    if (mem_map == NULL || new == NULL)
        return;

    if (*mem_map == NULL)
        *mem_map = new;
    else {
        unit = *mem_map;

        while (unit->next != NULL)
            unit = unit->next;

        unit->next = new;
    }
}


void free_mem_table(MemoryUnit *mem_map) {
    MemoryUnit *unit = mem_map;
    MemoryUnit *next;

    while (unit != NULL) {
        next = unit->next;
        free(unit);
        unit = next;
    }
}


void add_unit_and_increment_L(MemoryUnit **mem_map, MemoryUnit *new, int *L) {
    add_mem_unit_to_table(mem_map, new);
    (*L)++;
}