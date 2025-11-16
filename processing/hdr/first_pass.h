#ifndef __FIRST_PASS_H__
#define __FIRST_PASS_H__

#include "../hdr/passes.h"


#define EXTERN_LABEL_ADDRESS 0  /* default address */


/**
 * This is the function that implements the first
 * pass in assembler program.
 * The function receives:
 * - filename string
 * - address of IC variable
 * - address of DC variable
 * - address of memory map (table)
 * - address of labels map (table)
 * The function reads lines from file with expanded
 * macros (am file) and creates the tables:
 * - partial memory map (except labels encoding)
 *   that contains memory units with machine code
 *   and the addresses of each memory unit starting
 *   from IC initial value.
 * - labels map that contains all labels addresses
 *   and types (code/data) to complete the encoding
 *   in second pass.
 * Also, IC (instruction counter) and DC (data counter)
 * are counted and will be used for addresses updating
 * at the end of first pass and in second pass - using
 * their final values.
 * At the end of first pass, the data section is separated
 * from code section by adding to data labels the ICF value
 * so the data section will appear after the code section
 * in generated machine code on second pass.
 * If any errors occured, they will be redirected to the
 * user and the program will not proceed to second pass.
 */
int run_first_pass(char *, unsigned int *, unsigned int *, MemoryUnit **, Label **);

#endif