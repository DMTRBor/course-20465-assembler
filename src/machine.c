#include "../hdr/machine.h"


struct command commands[] = {
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


struct instruction instructions[] = {
    {".data",   1},
    {".string", 2},
    {".mat",    3},
    {".entry",  4},
    {".extern", 5}
};


LegalAddrMethods legal_addressing_methods[] = {
    /**
     * op | opcode | src addr.method | dst addr.method
     * example (first line):
     * {"mov", 0}  |  0, 1, 2, 3       1, 2, 3
     */
    { &commands[0],  {1, 1, 1, 1}, {0, 1, 1, 1} },    /* mov */
    { &commands[1],  {1, 1, 1, 1}, {1, 1, 1, 1} },    /* cmp */
    { &commands[2],  {1, 1, 1, 1}, {0, 1, 1, 1} },    /* add */
    { &commands[3],  {1, 1, 1, 1}, {0, 1, 1, 1} },    /* sub */
    { &commands[4],  {0, 1, 1, 0}, {0, 1, 1, 1} },    /* not */
    { &commands[5],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* clr */
    { &commands[6],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* lea */
    { &commands[7],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* inc */
    { &commands[8],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* dec */
    { &commands[9],  {0, 0, 0, 0}, {0, 1, 1, 1} },    /* jmp */
    { &commands[10], {0, 0, 0, 0}, {0, 1, 1, 1} },    /* bne */
    { &commands[11], {0, 0, 0, 0}, {0, 1, 1, 1} },    /* red */
    { &commands[12], {0, 0, 0, 0}, {0, 1, 1, 1} },    /* prn */
    { &commands[13], {0, 0, 0, 0}, {0, 1, 1, 1} },    /* jsr */
    { &commands[14], {0, 0, 0, 0}, {0, 0, 0, 0} },    /* rts */
    { &commands[15], {0, 0, 0, 0}, {0, 0, 0, 0} }     /* stop */
};


ExpectedNumOfOperands expected_num_of_operands[] = {
    { &commands[0],  TWO_OPERANDS },     /* mov */
    { &commands[1],  TWO_OPERANDS },     /* cmp */
    { &commands[2],  TWO_OPERANDS },     /* add */
    { &commands[3],  TWO_OPERANDS },     /* sub */
    { &commands[4],  SINGLE_OPERAND },   /* not */
    { &commands[5],  SINGLE_OPERAND },   /* clr */
    { &commands[6],  TWO_OPERANDS },     /* lea */
    { &commands[7],  SINGLE_OPERAND },   /* inc */
    { &commands[8],  SINGLE_OPERAND },   /* dec */
    { &commands[9],  SINGLE_OPERAND },   /* jmp */
    { &commands[10], SINGLE_OPERAND },   /* bne */
    { &commands[11], SINGLE_OPERAND },   /* red */
    { &commands[12], SINGLE_OPERAND },   /* prn */
    { &commands[13], SINGLE_OPERAND },   /* jsr */
    { &commands[14], NO_OPERANDS },      /* rts */
    { &commands[15], NO_OPERANDS }       /* stop */
};