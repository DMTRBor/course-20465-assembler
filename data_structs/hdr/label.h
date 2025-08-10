#ifndef __LABEL_H__
#define __LABEL_H__

#include "../../machine/hdr/machine.h"
#include "../../utils/hdr/utils.h"


/* labels map unit */
typedef struct Label {
    int type;  /* CODE or DATA */
    char *name;
    unsigned int address;
    struct Label *next;  /* pointer to next label */
} Label;


/* forward declarations */
Label* new_label(void);
void set_label_fields(Label *, int, unsigned int);
void set_label_type(Label **, char *, unsigned int);
void free_labels_table(Label *);
int add_label_to_table(Label **, Label *);
int is_label_exists(Label **, char *);
void update_data_labels_address(Label **, int);

#endif