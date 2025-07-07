#include "../hdr/errors_handling.h"


/* -------------------------- Pre-Assembler -------------------------- */

int is_macro_name_valid(char *line, int line_number) {
    int id;  /* array element index */

    /* check if command appears in macro name */
    for (id = 0; id < NUM_OF_COMMANDS; id++) {
        if (strstr(line, commands[id].name) != NULL) {
            fprintf(stderr, "Error in line %d: command '%s' found in line\n", line_number, commands[id].name);
            return FALSE;
        }
    }

    /* check if instruction appears in macro name */
    for (id = 0; id < NUM_OF_INSTRUCTIONS; id++) {
        if (strstr(line, instructions[id].name) != NULL) {
            fprintf(stderr, "Error in line %d: instruction '%s' found in line\n", line_number, instructions[id].name);
            return FALSE;
        }
    }
    return TRUE;
}