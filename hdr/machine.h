#ifndef __MACHINE_H__
#define __MACHINE_H__


struct operation {
    char *name;
    int code;
};


typedef enum {
    NO_OPERANDS,
    SINGLE_OPERAND,
    TWO_OPERANDS
};


typedef struct {
    struct operation *oper;
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
    MAT,        /* 10 */
    REG         /* 11 */
} AddrMethodCode;


typedef struct {
    unsigned int immediate_addressing : 1; /* bit 0 */
    unsigned int direct_addressing    : 1; /* bit 1 */
    unsigned int mat_addressing       : 1; /* bit 2 */
    unsigned int reg_addressing       : 1; /* bit 3 */
} AddrMethodIndex;


typedef struct {
    struct operation *oper;
    AddrMethodIndex legal_src_addr_method;
    AddrMethodIndex legal_dst_addr_method;
} LegalAddrMethods;


/* external declarations */
extern struct operation operations[];
extern const char *instructions[];
extern const char *registers[];
extern LegalAddrMethods legal_addressing_methods[];
extern ExpectedNumOfOperands expected_num_of_operands[];


#define MEMORY_SIZE 256

#define MAX_WORDS_IN_SENTENCE 5

#define NUM_OF_OPERATIONS 16
#define NUM_OF_INSTRUCTIONS 5

#define NUM_OF_REGISTERS 8

#endif