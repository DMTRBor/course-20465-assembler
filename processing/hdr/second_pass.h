#ifndef __SECOND_PASS_H__
#define __SECOND_PASS_H__

#include "../hdr/passes.h"
#include "../../utils/hdr/build_output.h"


/**
 * This is the function that implements the second
 * pass in assembler program.
 * The function receives:
 * - filename string
 * - address of IC variable, with final value
 * - address of DC variable, with final value
 * - address of memory map (table)
 * - address of labels map (table)
 * The function reads lines from file with expanded
 * macros (am file) and completes the encoding of
 * the labels that were built in first pass and are
 * stored in labels table.
 * The memory units that contain missing addresses
 * of labels are completed with addresses from labels
 * table. In case no errors detected during run, the
 * program builds the machine code files:
 * - object file (.ob)
 * - external file (.ext) - only if external labels included
 * - entries file (.ent) - only if entry labels included
 * The machine code is encoded in special base 4 and are
 * represented by alphabet letters:
 * - a = 0, b = 1, c = 2, d = 3
 * The object file contains the address in left column and
 * the code in right column, in base 4.
 * The entries and the external file contain the address
 * in left column and the code in right column, in base 4.
 * After this step, if no errors occured, STATUS_CODE_OK
 * will be returned and the assembler machine code is
 * completed and is ready for linkage and loading.
 */
int run_second_pass(char *, unsigned int *, unsigned int *, MemoryUnit **, Label **);

#endif