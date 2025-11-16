#include "processing/hdr/pre_assembler.h"
#include "processing/hdr/first_pass.h"
#include "processing/hdr/second_pass.h"


/**
 * This is the main program. The program implements an
 * assembler with two passes. It receives the assembler
 * source files via command line arguments and creates
 * a machine code that is required for further steps in
 * compilation process - linkage and loading. The program
 * uses a machine model and an assembly defined specifically
 * for this project purpose.
 * The translation from assembly source code to machine code
 * contains 3 main steps:
 * - pre-assembler
 * - first pass
 * - second pass
 * After the second pass there will be created machine code
 * files, encoded in special base 4 representation:
 * - object file (.ob)
 * - external file (.ext) - only if external labels included
 * - entries file (.ent) - only if entry labels included
 * In case there are errors with assembler source content, the
 * errors will be detected and redirected to the user, but the
 * machine code files will not be created as they will not be
 * useful in further compilation steps.
 * The program returns a status code, that indicates the
 * program didn't crash at any stage - this is an indication
 * for operating system that is executing the program and is
 * not related to the assembler processing itself - it warns
 * about incorrect execution or problem with operating system.
 * The assembler source files can be passed to the program in
 * sequence, one after another. If a file does not exist or
 * the content is erroneous, an error will be redirected and
 * the program will proceed to processing of the next file
 * in sequence of file names arguments, until no files remained.
 * The usage from command line (brackets for optional params):
 * "assembler <path/to/>file1 <path/to/>file2 <path/to/>file3"
*/
int main(int argc, char *argv[])
{
    char filename[MAX_FNAME_LEN];  /* to store filename with extension */
    int arg_id = 0;  /* command line arg index */
    FILE *fp;

    MemoryUnit *mem_map;  /* memory map */
    Label *labels;  /* labels map */

    unsigned int ic_value, dc_value;

    /* not enough arguments */
    if (argc < MIN_ALLOWED_ARGS) {
        fprintf(stderr, "Error!\nUsage: %s file1 file2 ...\n", argv[0]);
        exit(STATUS_CODE_ERR);
    }

	fprintf(stdout, "\n");  /* empty line before processing */

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

            mem_map = NULL;  /* memory map */
            labels = NULL;  /* labels map */

            if (run_first_pass(argv[arg_id], &ic_value, &dc_value, &mem_map, &labels) == STATUS_CODE_OK) {
                fprintf(stdout, "Running second pass for: %s\n", filename);
                /* second pass - using final IC and DC values */
                run_second_pass(argv[arg_id], &ic_value, &dc_value, &mem_map, &labels);
            }
            
            /* clean used memory, if allocated */
            if (mem_map != NULL)
                free_mem_table(mem_map);
            if (labels != NULL)
                free_labels_table(labels);
        }
		
		/* separate printed output to improve log readability */
		fprintf(stdout, "---------------------------------------------\n");
    }

    /* no critical errors occured during run */
    exit(STATUS_CODE_OK);
}
