#include "../hdr/pre_assembler.h"


/**
 * This is the main program - 
*/
int main(int argc, char *argv[])
{
    char filename[MAX_FNAME_LEN];  /* to store filename with extension */
    int arg_id = 0;
    FILE *fp;

    /* not enough arguments */
    if (argc < MIN_ALLOWED_ARGS) {
        fprintf(stderr, "Error!\nUsage: %s file1 file2 ...\n", argv[0]);
        exit(STATUS_CODE_ERR);
    }

    while (--argc > 0) {
        /* next filename */
        arg_id++;

        /* add file extension */
        strcpy(filename, argv[arg_id]);
        strcat(filename, ASMB_FILE_EXTEN);

        /* file doesn't exist or it's restricted for reading */
        if ((fp = open_file(filename, READ_FILE_PERMISSION)) == NULL)
            continue;

        /* empty file */
        else if (file_content_size(fp) == 0) {
            fprintf(stderr, "File is empty: %s\n", filename);
            fclose(fp);  /* close the file */
            continue;
        }

        /* file is valid - start processing */
        fprintf(stdout, "Processing file: %s\n", filename);

        /* pre-assembler */
        if (parse_assembler_source(fp, argv[arg_id]) == STATUS_CODE_OK) {
            /* first pass */
            printf("Here goes first pass...\n");
        }
    }

    /* no critical errors occured during run */
    exit(STATUS_CODE_OK);
}
