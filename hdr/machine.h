#ifndef __MACHINE__H__
#define __MACHINE_H__


struct command {
    char *name;
    int code;
};


struct instruction {
    char *name;
    int code;
};


typedef enum {
    NO_OPERANDS,
    SINGLE_OPERAND,
    TWO_OPERANDS
};


typedef struct {
    struct command *cmd;
    int expected_num_of_operands;
} ExpectedNumOfOperands;


/* word memory structure */
typedef struct {
    unsigned int encoding_type : 2;   /* E,R,A - bits 0-1 */
    unsigned int dest_operand  : 2;   /* bits 2-3 */
    unsigned int src_operand   : 2;   /* bits 4-5 */
    unsigned int opcode        : 4;   /* bits 6-9 */
} Word;


typedef enum {
    IMMEDIATE,  /* 00 */
    DIRECT,     /* 01 */
    MATRIX,     /* 10 */
    REGISTER    /* 11 */
} AddrMethodCode;


typedef struct {
    unsigned int immediate_addressing : 1; /* bit 0 */
    unsigned int direct_addressing    : 1; /* bit 1 */
    unsigned int mat_addressing       : 1; /* bit 2 */
    unsigned int reg_addressing       : 1; /* bit 3 */
} AddrMethodIndex;


typedef struct {
    struct command *cmd;
    AddrMethodIndex legal_src_addr_method;
    AddrMethodIndex legal_dst_addr_method;
} LegalAddrMethods;


/* external declarations */
extern struct command commands[];
extern struct instruction instructions[];
extern LegalAddrMethods legal_addressing_methods[];


#define MEMORY_SIZE 256

#define MIN_REGISTER_INDEX 0
#define MAX_REGISTER_INDEX 7

#define MAX_WORDS_IN_SENTENCE 5

#endif