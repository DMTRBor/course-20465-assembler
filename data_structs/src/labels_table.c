#include "../hdr/label.h"


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


void set_label_fields(Label *label, int type, unsigned int address) {
    if (label == NULL)
        return;
    
    /* label name set previously */
    label->type = type;
    label->address = address;
}


void set_label_type(Label **table, char *label_name, unsigned int label_type) {
    Label *current;
    
    if (table == NULL || *table == NULL || label_name == NULL)
        return;

    current = *table;
    /* search for label by name */
    while (current != NULL) {
        if (strcmp(current->name, label_name) == STR_EQUAL) {
            current->type = label_type;  /* update label type */
            return;
        }
        current = current->next;
    }
}


void free_labels_table(Label *table) {
    Label *current = table;
    Label *next;
    
    while (current != NULL) {
        next = current->next;
        
        if (current->name != NULL)
            free(current->name);
        
        free(current);
        current = next;
    }
}


int add_label_to_table(Label **table, Label *new_label) {
    Label *current;
    
    if (table == NULL || new_label == NULL)
        return STATUS_CODE_ERR;

    if (*table == NULL)
        *table = new_label;
    else {
        current = *table;

        while (current->next != NULL)
            current = current->next;

        current->next = new_label;
    }

    return STATUS_CODE_OK;
}


int is_label_exists(Label **table, char *label_name) {
    Label *current;
    
    /* check if labels table in not empty and label name is valid */
    if (table == NULL || *table == NULL || label_name == NULL)
        return FALSE;

    current = *table;
    /* search in labels table */
    while (current != NULL) {
        if (strcmp(current->name, label_name) == STR_EQUAL)
            return TRUE;  /* label exists */
        /* go to next label */
        current = current->next;
    }
    
    return FALSE;  /* label not found */
}


unsigned int update_data_labels_address(Label **table, int ICF) {
    Label *current = *table;
    unsigned int DCF = INVALID_DCF;  /* data final counter */

    while (current != NULL) {
        if (current->type == DATA) {  /* detected data label */
            current->address += ICF;
            DCF = current->address;  /* update data final counter */
        }

        current = current->next;
    }

    return DCF;  /* return DCF */
}