#ifndef __FIRST_PASS_H__
#define __FIRST_PASS_H__

#include <stdio.h>

#include "../hdr/utils.h"
#include "../hdr/data_struct.h"
#include "../hdr/tables.h"
#include "../hdr/errors_handling.h"


#define MAX_LABELS_ALLOWED 255


int run_first_pass(char *, int *, int *);

#endif