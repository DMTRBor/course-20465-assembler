#ifndef __TABLES_H__
#define __TABLES_H__

#include <stdlib.h>
#include <string.h>

#include "../hdr/data_struct.h"
#include "../hdr/machine.h"


/* machine code type */
enum SectionType {
    CODE,
    DATA
};


/* labels map unit */
typedef struct Label {
    int type;  /* CODE or DATA */
    char *name;
    unsigned int address;
    struct Label *next;  /* pointer to next label */
} Label;


/* memory map unit */
typedef struct MemoryUnit {
    int type;  /* CODE or DATA */
    unsigned int address;
    Word encoded_value;
    struct MemoryUnit *next;
} MemoryUnit;


#define IC_INITIAL_VALUE 100
#define DC_INITIAL_VALUE 0

/* ----------- Memory Table Functions ----------- */
MemoryUnit* new_mem_unit(void);
void set_mem_unit_type(MemoryUnit *, int);
void set_mem_unit_addr(MemoryUnit *, unsigned int);
void set_word_fields(MemoryUnit *,
                     unsigned int, 
                     unsigned int,
                     unsigned int, 
                     unsigned int);
void add_mem_unit_to_table(MemoryUnit **, MemoryUnit *);
void free_mem_table(MemoryUnit *head);
void add_unit_and_increment_L(MemoryUnit **, MemoryUnit *, int *);

/* ----------- Labels Table Functions ----------- */
Label* new_label(void);
void set_label_fields(Label *, int, char *, unsigned int);
void free_labels_table(Label *);
int add_label_to_table(Label **, Label *);
int is_label_exist(Label *, Label *);

#endif