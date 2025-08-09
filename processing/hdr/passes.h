#ifndef __PASSES_H__
#define __PASSES_H__

#include "../../data_structs/hdr/line.h"
#include "../../data_structs/hdr/label.h"
#include "../../data_structs/hdr/mem_unit.h"
#include "../../encoding/hdr/code_section.h"
#include "../../encoding/hdr/data_section.h"


#define EXTERN_LABEL_ADDRESS 0
#define WHITESPACE_CHAR ' '


int run_first_pass(char *, unsigned int *, unsigned int *, MemoryUnit **, Label **);
int run_second_pass(char *, unsigned int *, unsigned int *, MemoryUnit **, Label **);

#endif