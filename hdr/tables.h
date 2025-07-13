#ifndef __TABLES_H__
#define __TABLES_H__

#include "../hdr/data_struct.h"
#include "../hdr/machine.h"


/* machine code type */
typedef enum {
    CODE,
    DATA
};


/* labels map unit */
typedef struct Label {
    int type;  /* CODE or DATA */
    char *name;
    unsigned int address;
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

#endif