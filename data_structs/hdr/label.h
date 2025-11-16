#ifndef __LABEL_H__
#define __LABEL_H__

#include "../../machine/hdr/machine.h"
#include "../../utils/hdr/utils.h"


#define DEFAULT_LABEL_ADDR 0  /* default address */


/* labels map unit (node) */
typedef struct Label {
    int type;  /* CODE or DATA */
    char *name;
    unsigned int address;
    struct Label *next;  /* pointer to next label */
} Label;


/**
 * This function creates a new label
 * unit, when it's fields are
 * initialized with default values.
 * It receives no arguments and returns
 * a address of the newly created label
 * unit (node).
 */
Label* new_label(void);


/**
 * This function receives:
 * - address of the label unit
 * - label type code
 * - label address
 * It sets the type and the address fields
 * with the received values.
 * The function does not return any value.
 */
void set_label_fields(Label *, int, unsigned int);


/**
 * The function receives:
 * - address of the head of labels table (list)
 * - label name string
 * - label type code
 * It searches for the label by name in the
 * table and if found, the label type is
 * updated to the parameter received.
 * The function does not return any value.
 */
void set_label_type(Label **, char *, unsigned int);


/**
 * The function receives address of the head of
 * labels table, and frees the table by
 * deallocating all the memory allocated for the
 * labels table (linked list of label nodes)
 * including the fields of each label, if needed.
 * The function does not return any value.
 */
void free_labels_table(Label *);


/**
 * The function receives:
 * - address of the head of labels table
 * - address of the label (node)
 * It connects the label at the tail of the
 * labels table.
 * The function returns STATUS_CODE_OK if the
 * label was added at the end of the table
 * successfully. Otherwise, it returns
 * STATUS_CODE_ERR, that indicates failure. 
 */
int add_label_to_table(Label **, Label *);


/**
 * The function receives:
 * - address of the head of labels table
 * - name of a label
 * It checks if a label with a given name
 * exists in the table.
 * If a label with a given name exists in
 * the table, will be returned the code TRUE.
 * Otherwise, FALSE code will be returned.
 */
int is_label_exists(Label **, char *);


/**
 * The function receives:
 * - address of the head of labels table
 * - ICF (instruction counter final value),
 *   that is computed in the first pass
 * It updates the addresses of labels marked
 * as data with ICF value - this completes the
 * separation of code and data section at
 * the end of first pass.
 * The function does not return any value.
 */
void update_data_labels_address(Label **, int);

#endif