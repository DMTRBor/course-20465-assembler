#ifndef __TABLES__H__
#define __TABLES__H__

#include "../hdr/data_struct.h"


/* labels table */
typedef struct Label {
    char *name;
    unsigned int address;
    char *type;
} Label;


#define IC_INITIAL_VALUE 100
#define DC_INITIAL_VALUE 0

#endif