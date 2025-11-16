#ifndef __MACHINE_H__
#define __MACHINE_H__


/* machine code type */
enum SectionType {
    CODE,
    DATA,
    EXTERNAL,
    ENTRY
};


struct operation {
    char *name;  /* operation name */
    int code;    /* operation code */
};


/* number of operands in operation
   classification */
enum OpNumOfOperands{
    NO_OPERANDS,
    DEST_ONLY,
    SRC_AND_DEST
};


/* expected num of
   operands for operation */
typedef struct {
    struct operation *oper;  /* operation name and code */
    int expected_num_of_operands;
} ExpectedNumOfOperands;


typedef enum {
    A,  /* Absolute */
    E,  /* External */
    R   /* Relocatable */
} EncodingType;


/* word memory structure */
/* 10-bit unsigned integer */
typedef struct {
    unsigned int encoding_type : 2;   /* E,R,A - bits 0-1 */
    unsigned int dest_operand  : 2;   /* bits 2-3 */
    unsigned int src_operand   : 2;   /* bits 4-5 */
    unsigned int opcode        : 4;   /* bits 6-9 */
} Word;


/* addressing methods enumeration
   for encoding in memory */
typedef enum {
    IMMEDIATE,  /* 00 */
    DIRECT,     /* 01 */
    MAT,        /* 10 */
    REG         /* 11 */
} AddrMethodCode;


/* representation of addressing
   methods, using index number as code */
typedef struct {
    unsigned int immediate_addressing : 1; /* bit 0 */
    unsigned int direct_addressing    : 1; /* bit 1 */
    unsigned int mat_addressing       : 1; /* bit 2 */
    unsigned int reg_addressing       : 1; /* bit 3 */
} AddrMethodIndex;


typedef struct {
    struct operation *oper;  /* operation name and code */
    AddrMethodIndex legal_src_addr_method;
    AddrMethodIndex legal_dst_addr_method;
} LegalAddrMethods;


/* external declarations, used in source file */
extern struct operation operations[];
extern const char *directives[];
extern const char *registers[];
extern LegalAddrMethods legal_addressing_methods[];
extern ExpectedNumOfOperands expected_num_of_operands[];
extern const char base4_encoded[];

/* macros */
#define MEMORY_SIZE 256
#define MIN_INTEGER -128
#define MAX_INTEGER 127

#define MAX_WORDS_IN_SENTENCE 5

#define NUM_OF_OPERATIONS 16
#define NUM_OF_DIRECTIVES 5

#define NUM_OF_REGISTERS 8

/**
 * range of values for 10-bit
 * signed integer, using 2's complement
 */
#define MAX_10_BIT_VALUE 511
#define MIN_10_BIT_VALUE -512

#define IC_RESET_VALUE 100  /* IC initial value */
#define DC_RESET_VALUE 0    /* DC initial value */

#endif