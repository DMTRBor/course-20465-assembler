#include "../hdr/machine.h"


struct operation operations[] = {
    {"mov", 0},
    {"cmp", 1},
    {"add", 2},
    {"sub", 3},
    {"not", 4},
    {"clr", 5},
    {"lea", 6},
    {"inc", 7},
    {"dec", 8},
    {"jmp", 9},
    {"bne", 10},
    {"red", 11},
    {"prn", 12},
    {"jsr", 13},
    {"rts", 14},
    {"stop", 15}
};


const char *instructions[NUM_OF_INSTRUCTIONS] = {
    ".data",
    ".string",
    ".mat",
    ".entry",
    ".extern"
};


const char *registers[NUM_OF_REGISTERS] = {
    "r0",
    "r1",
    "r2",
    "r3",
    "r4",
    "r5",
    "r6",
    "r7"
};


LegalAddrMethods legal_addressing_methods[] = {
    /**
     * op | opcode | src addr.method | dst addr.method
     * example (first line):
     * {"mov", 0}  |  0, 1, 2, 3       1, 2, 3
     */
    { &operations[0],  {1, 1, 1, 1}, {0, 1, 1, 1} },    /* mov */
    { &operations[1],  {1, 1, 1, 1}, {1, 1, 1, 1} },    /* cmp */
    { &operations[2],  {1, 1, 1, 1}, {0, 1, 1, 1} },    /* add */
    { &operations[3],  {1, 1, 1, 1}, {0, 1, 1, 1} },    /* sub */
    { &operations[4],  {0, 1, 1, 0}, {0, 1, 1, 1} },    /* not */
    { &operations[5],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* clr */
    { &operations[6],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* lea */
    { &operations[7],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* inc */
    { &operations[8],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* dec */
    { &operations[9],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* jmp */
    { &operations[10], {0, 0, 0, 0}, {0, 1, 1, 1} },    /* bne */
    { &operations[11], {0, 0, 0, 0}, {0, 1, 1, 1} },    /* red */
    { &operations[12], {0, 0, 0, 0}, {0, 1, 1, 1} },    /* prn */
    { &operations[13], {0, 0, 0, 0}, {0, 1, 1, 1} },    /* jsr */
    { &operations[14], {0, 0, 0, 0}, {0, 0, 0, 0} },    /* rts */
    { &operations[15], {0, 0, 0, 0}, {0, 0, 0, 0} }     /* stop */
};


ExpectedNumOfOperands expected_num_of_operands[] = {
    { &operations[0],  TWO_OPERANDS },     /* mov */
    { &operations[1],  TWO_OPERANDS },     /* cmp */
    { &operations[2],  TWO_OPERANDS },     /* add */
    { &operations[3],  TWO_OPERANDS },     /* sub */
    { &operations[4],  SINGLE_OPERAND },   /* not */
    { &operations[5],  SINGLE_OPERAND },   /* clr */
    { &operations[6],  TWO_OPERANDS },     /* lea */
    { &operations[7],  SINGLE_OPERAND },   /* inc */
    { &operations[8],  SINGLE_OPERAND },   /* dec */
    { &operations[9],  SINGLE_OPERAND },   /* jmp */
    { &operations[10], SINGLE_OPERAND },   /* bne */
    { &operations[11], SINGLE_OPERAND },   /* red */
    { &operations[12], SINGLE_OPERAND },   /* prn */
    { &operations[13], SINGLE_OPERAND },   /* jsr */
    { &operations[14], NO_OPERANDS },      /* rts */
    { &operations[15], NO_OPERANDS }       /* stop */
};