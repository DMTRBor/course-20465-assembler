#include "../hdr/pre_assembler.h"
#include "../hdr/first_pass.h"


/**
 * This is the main program - 
*/
int main(int argc, char *argv[])
{
    char filename[MAX_FNAME_LEN];  /* to store filename with extension */
    int arg_id = 0;
    FILE *fp;

    unsigned int ic_value, dc_value;

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
        if (run_pre_assembler(fp, argv[arg_id]) == STATUS_CODE_OK) {
            /* first pass */
            fprintf(stdout, "Running first pass for: %s\n", filename);
            /* initialize IC and DC with reset value */
            ic_value = IC_RESET_VALUE;
            dc_value = DC_RESET_VALUE;

            MemoryUnit *mem = NULL;  /* memory map */
            Label *labels = NULL;  /* labels map */
    
            if (run_first_pass(argv[arg_id], &ic_value, &dc_value, &mem, &labels) == STATUS_CODE_OK) {
                fprintf(stdout, "Running second pass for: %s\n", filename);
                /* second pass - using final IC and DC values */
                /* run_second_pass(argv[arg_id], &ic_value, &dc_value); */
            }
            
            /* clean used memory, if allocated */
            if (mem != NULL)
                free_mem_table(mem);
            if (labels != NULL)
                free_labels_table(labels);
        }
    }

    /* no critical errors occured during run */
    exit(STATUS_CODE_OK);
}
