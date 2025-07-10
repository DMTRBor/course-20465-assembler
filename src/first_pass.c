#include "../hdr/first_pass.h"


int run_first_pass(char *filename) {
    int line_number = 1;

    char am_filename[MAX_FNAME_LEN];
    FILE *fp;

    Line *curr_line = NULL;

    /* add macro-parsed file extension */
    strcpy(am_filename, filename);
    strcat(am_filename, ASMB_MCRO_FILE_EXTEN);

    /* open *.am file for reading */
    if ((fp = open_file(am_filename, READ_FILE_PERMISSION)) == NULL)
        return STATUS_CODE_ERR;

    /* create file lines structured list */
    /* point to first line */
    curr_line = file_to_list(fp);

    if (curr_line == NULL) {  /* memory misallocation */
        fclose(fp);
        return STATUS_CODE_ERR;
    }
    fclose(fp);

    while (curr_line != NULL) {
        /* go to next line */
        curr_line = curr_line->next;
    }
    
    /* free used memory */
    free_list(curr_line);

    return STATUS_CODE_OK;
}