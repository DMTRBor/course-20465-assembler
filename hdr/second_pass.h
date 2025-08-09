#ifndef __SECOND_PASS_H__
#define __SECOND_PASS_H__

#include "../hdr/data_struct.h"
#include "../hdr/tables.h"
#include "../hdr/encoding.h"


#define WHITESPACE_CHAR ' '


int run_second_pass(char *, unsigned int *, unsigned int *, MemoryUnit **, Label **);


#endif