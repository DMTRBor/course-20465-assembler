#ifndef __COMMANDS__H__
#define __COMMANDS__H__

#include "../hdr/mem_map.h"

struct command {
    char *name;
    int code;
};

struct instruction {
    char *name;
    int code;
};

typedef struct {
    unsigned int immediate_addressing : 1; /* bit 0 */
    unsigned int direct_addressing    : 1; /* bit 1 */
    unsigned int mat_addressing       : 1; /* bit 2 */
    unsigned int reg_addressing       : 1; /* bit 3 */
} AddrMethod;

typedef struct {
    struct command *cmd;
    AddrMethod legal_src_addr_method;
    AddrMethod legal_dst_addr_method;
} LegalAddrMethods;

/* external declarations */
extern struct command commands[];
extern struct instruction instructions[];
extern LegalAddrMethods legal_addressing_methods[];

#define NUM_OF_COMMANDS 16
#define NUM_OF_INSTRUCTIONS 5

#endif