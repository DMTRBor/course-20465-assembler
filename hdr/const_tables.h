#ifndef __COMMANDS__H__
#define __COMMANDS__H__

struct command {
    char *name;
    int code;
};

struct instruction {
    char *name;
    int code;
};

/* external declarations */
extern struct command commands[];
extern struct instruction instructions[];

#define NUM_OF_COMMANDS 16
#define NUM_OF_INSTRUCTIONS 5

#endif