#ifndef __PASSES_H__
#define __PASSES_H__

#include "../../data_structs/hdr/line.h"
#include "../../data_structs/hdr/label.h"
#include "../../data_structs/hdr/mem_unit.h"
#include "../../encoding/hdr/code_section.h"
#include "../../encoding/hdr/data_section.h"


/* directives */
#define DATA_DIRECTIVE ".data"
#define STRING_DIRECTIVE ".string"
#define MAT_DIRECTIVE ".mat"
#define ENTRY_DIRECTIVE ".entry"
#define EXTERNAL_DIRECTIVE ".extern"

#endif