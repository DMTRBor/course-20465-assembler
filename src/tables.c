#include "../hdr/tables.h"


/* ---------------------------- Memory Table ---------------------------- */

MemoryUnit* new_mem_unit(void) {
    MemoryUnit *new_unit = (MemoryUnit*)malloc(sizeof(MemoryUnit));
    /* check if allocated properly */
    if (new_unit == NULL)
        return NULL;
    
    /* initialize fields with default values */
    new_unit->type = CODE;
    new_unit->address = 0;
    memset(&new_unit->encoded_value, 0, sizeof(Word));  /* reset word fields */
    new_unit->next = NULL;
    
    return new_unit;
}


void set_mem_unit_type(MemoryUnit *unit, int type) {
    if (unit == NULL)
        return;
    
    unit->type = type;
}


void set_mem_unit_addr(MemoryUnit *unit, unsigned int address) {
    if (unit == NULL)
        return;
    
    unit->address = address;
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


void add_mem_unit_to_table(MemoryUnit **head, MemoryUnit *new) {
    MemoryUnit *current;
    
    if (head == NULL || new == NULL)
        return;
    
    if (*head == NULL)
        *head = new;
    else {
        current = *head;

        while (current->next != NULL)
            current = current->next;

        current->next = new;
    }
}


void free_mem_table(MemoryUnit *head) {
    MemoryUnit *current = head;
    MemoryUnit *next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}


/* ---------------------------- Labels Table ---------------------------- */

Label* new_label(void) {
    Label *new_label = (Label*)malloc(sizeof(Label));
    /* check if allocated properly */
    if (new_label == NULL)
        return NULL;
    
    /* initialize fields with default values */
    new_label->type = CODE;
    new_label->name = NULL;
    new_label->address = 0;
    new_label->next = NULL;
    
    return new_label;
}


void set_label_fields(Label *label, int type, char *name, unsigned int address) {
    if (label == NULL)
        return;
    
    label->type = type;
    label->address = address;
    
    /* allocate and copy new name */
    label->name = (char*)malloc(strlen(name) + 1);
    strcpy(label->name, name);
}


void free_labels_table(Label *head) {
    Label *current = head;
    Label *next;
    
    while (current != NULL) {
        next = current->next;
        
        if (current->name != NULL)
            free(current->name);
        
        free(current);
        current = next;
    }
}


void add_label_to_table(Label **tail, Label *new_label) {
    if (tail == NULL || new_label == NULL)
        return;
    
    if (*tail == NULL)
        *tail = new_label;
    else {
        (*tail)->next = new_label;
        *tail = new_label;
    }
}


int is_label_exist(Label *label, Label *head) {
    Label *current = head;
    
    if (head == NULL || label == NULL)
        return FALSE;
    
    /* search in labels table */
    while (current != NULL) {
        if (current->type == label->type &&
            current->address == label->address &&
            strcmp(current->name, label->name) == STR_EQUAL) {
            return TRUE;  /* label exists */
        }
        current = current->next;
    }
    
    return FALSE;  /* label not found */
}