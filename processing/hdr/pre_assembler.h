#ifndef __PRE_ASSEMBLER_H__
#define __PRE_ASSEMBLER_H__

#include "../../utils/hdr/utils.h"
#include "../../data_structs/hdr/line.h"
#include "../../data_structs/hdr/macro.h"
#include "../../utils/hdr/errors_handling.h"


#define MAX_MACROS_ALLOWED 50

#define NUM_OF_MCRO_ARGS 2     /* macro opening expected number of arguments */
#define NUM_OF_MCROEND_ARGS 1  /* macro closing expected number of arguments */


/**
 * This is the function that implements the pre-
 * assembler processing step.
 * The function receives:
 * - pointer to a stream of lines from file
 * - filename string
 * The function reads each line and checks if there
 * is a macro definition present - if yes, macro
 * lines are added to macro lines list, and to
 * macros list, in case there are numerous different
 * macros. Then, for each macro call in the source
 * assembler file, the macro call line is replaced
 * with macro lines from macro lines list with the
 * corresponding macro name, at each call. This
 * process is called macro expansion.
 * If no macros appear in the source file, the lines
 * list at the end of the pre-assembler processing
 * are similar to source file, as no macros needed to
 * be expanded. The lines list with the expanded macros
 * are written to a file with .am extension, in case no
 * serious errors occured during pre-assembler. If some
 * errors occured, they will be redirected to the
 * user and error code STATUS_CODE_ERR will
 * be returned, avoiding proceeding to first pass.
 * Otherwise, first pass process starts at next step.
 */
int run_pre_assembler(FILE *, char *);

#endif