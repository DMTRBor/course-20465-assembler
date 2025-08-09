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


void set_mem_unit_addresses(MemoryUnit **table, unsigned int start_address) {
    MemoryUnit *current = *table;
    unsigned int current_address = start_address;
    
    while (current != NULL) {
        current->address = current_address++;
        current = current->next;
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


void add_mem_unit_to_table(MemoryUnit **table, MemoryUnit *new) {
    MemoryUnit *current;
    
    if (table == NULL || new == NULL)
        return;

    if (*table == NULL)
        *table = new;
    else {
        current = *table;

        while (current->next != NULL)
            current = current->next;

        current->next = new;
    }
}


void free_mem_table(MemoryUnit *table) {
    MemoryUnit *current = table;
    MemoryUnit *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}


void add_unit_and_increment_L(MemoryUnit **table, MemoryUnit *new, int *L) {
    add_mem_unit_to_table(table, new);
    (*L)++;
}