#ifndef __SECOND_PASS_H__
#define __SECOND_PASS_H__

#include "../hdr/passes.h"
#include "../../utils/hdr/build_output.h"


#define WHITESPACE_CHAR ' '

/* forward declarations */
int run_second_pass(char *, unsigned int *, unsigned int *, MemoryUnit **, Label **);

#endif