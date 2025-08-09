#include "../hdr/machine.h"


struct operation operations[] = {
    {"mov", 0},
    {"cmp", 1},
    {"add", 2},
    {"sub", 3},
    {"lea", 4},
    {"clr", 5},
    {"not", 6},
    {"inc", 7},
    {"dec", 8},
    {"jmp", 9},
    {"bne", 10},
    {"jsr", 11},
    {"red", 12},
    {"prn", 13},
    {"rts", 14},
    {"stop", 15}
};


const char *directives[NUM_OF_DIRECTIVES] = {
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
    { &operations[4],  {0, 1, 1, 0}, {0, 1, 1, 1} },    /* lea */
    { &operations[5],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* clr */
    { &operations[6],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* not */
    { &operations[7],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* inc */
    { &operations[8],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* dec */
    { &operations[9],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* jmp */
    { &operations[10], {0, 0, 0, 0}, {0, 1, 1, 1} },    /* bne */
    { &operations[11], {0, 0, 0, 0}, {0, 1, 1, 1} },    /* jsr */
    { &operations[12], {0, 0, 0, 0}, {0, 1, 1, 1} },    /* red */
    { &operations[13], {0, 0, 0, 0}, {1, 1, 1, 1} },    /* prn */
    { &operations[14], {0, 0, 0, 0}, {0, 0, 0, 0} },    /* rts */
    { &operations[15], {0, 0, 0, 0}, {0, 0, 0, 0} }     /* stop */
};


ExpectedNumOfOperands expected_num_of_operands[] = {
    { &operations[0],  SRC_AND_DEST },     /* mov */
    { &operations[1],  SRC_AND_DEST },     /* cmp */
    { &operations[2],  SRC_AND_DEST },     /* add */
    { &operations[3],  SRC_AND_DEST },     /* sub */
    { &operations[4],  SRC_AND_DEST },     /* lea */
    { &operations[5],  DEST_ONLY },        /* clr */
    { &operations[6],  DEST_ONLY },        /* not */
    { &operations[7],  DEST_ONLY },        /* inc */
    { &operations[8],  DEST_ONLY },        /* dec */
    { &operations[9],  DEST_ONLY },        /* jmp */
    { &operations[10], DEST_ONLY },        /* bne */
    { &operations[11], DEST_ONLY },        /* jsr */
    { &operations[12], DEST_ONLY },        /* red */
    { &operations[13], DEST_ONLY },        /* prn */
    { &operations[14], NO_OPERANDS },      /* rts */
    { &operations[15], NO_OPERANDS }       /* stop */
};


/* base-4 to letters mapping */
const char base4_encoded[] = {'a', 'b', 'c', 'd'};