#ifndef __FIRST_PASS_H__
#define __FIRST_PASS_H__

#include <stdio.h>

#include "../hdr/data_struct.h"
#include "../hdr/tables.h"
#include "../hdr/encoding.h"

/* relevant only for first pass */
#define EXTERN_LABEL_ADDRESS 0


int run_first_pass(char *, unsigned int *, unsigned int *, MemoryUnit **, Label **);

#endif