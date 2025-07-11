#include "../hdr/const_tables.h"


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
    { &commands[0],  {1, 1, 1, 1}, {0, 1, 1, 1} },
    { &commands[1],  {1, 1, 1, 1}, {1, 1, 1, 1} },
    { &commands[2],  {1, 1, 1, 1}, {0, 1, 1, 1} },
    { &commands[3],  {1, 1, 1, 1}, {0, 1, 1, 1} },
    { &commands[4],  {0, 1, 1, 0}, {0, 1, 1, 1} },
    { &commands[5],  {0, 0, 0, 0}, {0, 1, 1, 1} },
    { &commands[6],  {0, 0, 0, 0}, {0, 1, 1, 1} },
    { &commands[7],  {0, 0, 0, 0}, {0, 1, 1, 1} },
    { &commands[8],  {0, 0, 0, 0}, {0, 1, 1, 1} },
    { &commands[9],  {0, 0, 0, 0}, {0, 1, 1, 1} },
    { &commands[10], {0, 0, 0, 0}, {0, 1, 1, 1} },
    { &commands[11], {0, 0, 0, 0}, {0, 1, 1, 1} },
    { &commands[12], {0, 0, 0, 0}, {0, 1, 1, 1} },
    { &commands[13], {0, 0, 0, 0}, {0, 1, 1, 1} },
    { &commands[14], {0, 0, 0, 0}, {0, 0, 0, 0} },
    { &commands[15], {0, 0, 0, 0}, {0, 0, 0, 0} }
};