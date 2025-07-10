#ifndef __MEM_MAP__H__
#define __MEM_MAP__H__

typedef struct {
    unsigned int encoding_type : 2;   /* E,R,A - bits 0-1 */
    unsigned int dest_operand  : 2;   /* bits 2-3 */
    unsigned int src_operand   : 2;   /* bits 4-5 */
    unsigned int opcode        : 4;   /* bits 6-9 */
} word;

/* register 10-bit word representation */
typedef struct {
    const char *name;
    word reg_word;
} RegisterMap;

#define MEMORY_SIZE 256
#define NUM_OF_OPERANDS 3
#define MAX_WORDS_IN_SENTENCE 5

#endif