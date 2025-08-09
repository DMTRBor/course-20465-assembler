#ifndef __LINE_H__
#define __LINE_H__

#include "../../utils/hdr/utils.h"


/* a line in file */
typedef struct Line {
    char *line;
    struct Line *next;
} Line;


/* forward declarations */
Line* new_line(char *);
void delete_line_from_list(Line *);
void free_list(Line *);
Line* file_to_list(FILE *);
int list_to_file(Line *, FILE *);

#endif