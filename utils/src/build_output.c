#include "../hdr/build_output.h"


void decimal_to_base4(unsigned int decimal, char *converted) {
    /* temporary converted number storage */
    char temp[MAX_DIGITS_BASE_4 + 1];  /* +1 for null terminator */
    int i = 0, j;

    /* the decimal is zero */
    if (decimal == 0) {
        converted[0] = base4_encoded[0];  /* single digit */
        converted[1] = NULL_TERMINATOR;
        return;
    }
    
    /* store reversed conversion of decimal to base 4 */
    while (decimal > 0) {
        temp[i++] = base4_encoded[decimal % BASE_4];  /* map digit to letter */
        decimal /= BASE_4;
    }
    temp[i] = NULL_TERMINATOR;
    
    /* revert the number, skip null terminator */
    for (j = i - 1; j >= 0; j--) {
        *converted = temp[j];
        converted++;
    }
    *converted = NULL_TERMINATOR;
}


void build_object_file(char *filename, unsigned int ICF, unsigned int DCF, MemoryUnit **mem, Label **labels) {
    /* implementation */
}


void build_ext_file(char *filename, Label **labels) {
    /* implementation */
}


int build_ent_file(char *filename, Label **labels) {
    Label *label = *labels;
    FILE *fp;

    char *base4_address;  /* buffer for base 4 address */

    char ent_filename[MAX_FNAME_LEN];
    char entry_content[MAX_FILE_BUFFER] = "";  /* buffer to store all entry labels */
    char line[MAX_LINE_LEN];  /* buffer for single label */

    int has_entries = FALSE;  /* flag to check if any entry labels exist */

    /* search for entry labels in labels table */
    while (label != NULL) {
        /* check if entry label */
        if (label->type == ENTRY) {
            /* allocate memory for converted base 4 address */
            base4_address = (char *)malloc(MAX_DIGITS_BASE_4 + 1);

            decimal_to_base4(label->address, base4_address);
            /* add entry line */
            sprintf(line, "%s\t%s\n", label->name, base4_address);
            strcat(entry_content, line);  /* append to entry content */

            has_entries = TRUE;  /* found at least one entry label */
        }
        /* move to next label */
        label = label->next;
    }

    /* only create file if there are entry labels */
    if (has_entries) {
        /* create entry filename */
        strcpy(ent_filename, filename);
        strcat(ent_filename, ENTRIES_FILE_EXTEN);

        /* open file for writing */
        if ((fp = open_file(ent_filename, WRITE_FILE_PERMISSION)) == NULL) {
            fprintf(stderr, "Error - cannot create entry file: %s\n", ent_filename);
            free(base4_address);  /* free allocated memory */
            return STATUS_CODE_ERR;
        }

        /* write entry content to file */
        if (fprintf(fp, "%s", entry_content) < MIN_CHARS_WRITTEN) {
            fprintf(stderr, "Error writing to entry file: %s\n", ent_filename);
            free(base4_address);  /* free allocated memory */
            fclose(fp);
            return STATUS_CODE_ERR;
        }

        fclose(fp);  /* close file */
    }

    free(base4_address);  /* free allocated memory */
    return STATUS_CODE_OK;  /* encoding labels addresses succeeded */
}