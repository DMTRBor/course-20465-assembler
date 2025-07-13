#ifndef __PRE_ASSEMBLER_H__
#define __PRE_ASSEMBLER_H__

#include "../hdr/utils.h"
#include "../hdr/data_struct.h"
#include "../hdr/errors_handling.h"


#define MAX_MACROS_ALLOWED 50

#define NUM_OF_MCRO_ARGS 2
#define NUM_OF_MCROEND_ARGS 1


int run_pre_assembler(FILE *, char *);

#endif