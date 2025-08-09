#ifndef __DATA_SECTION_H__
#define __DATA_SECTION_H__


#include "../hdr/util.h"


/* Data */
int encode_data_direc(char *, MemoryUnit **, int);
int encode_string_direc(char *, MemoryUnit **, int);
int encode_mat_direc(char *, MemoryUnit **, int);

#endif