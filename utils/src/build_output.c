#include "../hdr/build_output.h"


void decimal_to_base4(unsigned int decimal, char *converted) {
    /* temporary converted number storage */
    char temp[MAX_DIGITS_BASE_4 + 1];  /* +1 for null terminator */
    int i = 0, j;  /* indices */

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


int write_to_file(char *filename, char *content, char *extension) {
    FILE *fp;  /* file pointer */
    /* full filename */
    char full_filename[MAX_FNAME_LEN];

    /* create filename with extension */
    strcpy(full_filename, filename);
    strcat(full_filename, extension);

    /* open file for writing */
    if ((fp = open_file(full_filename, WRITE_FILE_PERMISSION)) == NULL) {
        fprintf(stderr, "Error - cannot create file: %s\n", full_filename);
        return STATUS_CODE_ERR;
    }

    /* write content to file */
    if (fprintf(fp, "%s", content) < MIN_CHARS_WRITTEN) {
        fprintf(stderr, "Error writing to file: %s\n", full_filename);
        fclose(fp);
        return STATUS_CODE_ERR;
    }

    fclose(fp);  /* close file */
    return STATUS_CODE_OK;
}


int build_object_file(char *filename, unsigned int ICF, unsigned int DCF, MemoryUnit **mem_map, Label **labels) {
    return STATUS_CODE_OK;
}


int build_ext_file(char *filename, MemoryUnit **mem_map) {
    MemoryUnit *mem_unit = *mem_map;

    char *base4_address;  /* buffer for base 4 address */

    char ext_content[MAX_FILE_BUFFER] = "";  /* buffer to store all external labels */
    char line[MAX_LINE_LEN];  /* buffer for single label */

    /* search for external labels in memory table */
    while (mem_unit != NULL) {
        /* check if external label */
        if (mem_unit->label_op_name != NULL && mem_unit->encoded_value.encoding_type == E) {
            /* allocate memory for converted base 4 address */
            if ((base4_address = (char *)malloc(MAX_DIGITS_BASE_4 + 1)) == NULL) {
                fprintf(stderr, "Memory allocation error for base 4 address\n");
                return STATUS_CODE_ERR;
            }

            decimal_to_base4(mem_unit->address, base4_address);
            /* add external line */
            sprintf(line, "%s\t%s\n", mem_unit->label_op_name, base4_address);
            strcat(ext_content, line);  /* append to external content */

            free(base4_address);  /* free allocated memory */
        }
        /* move to next memory unit */
        mem_unit = mem_unit->next;
    }

    /* write content to file */
    return write_to_file(filename, ext_content, EXTERNAL_FILE_EXTEN);
}


int build_ent_file(char *filename, Label **labels) {
    Label *label = *labels;

    char *base4_address;  /* buffer for base 4 address */

    char entry_content[MAX_FILE_BUFFER] = "";  /* buffer to store all entry labels */
    char line[MAX_LINE_LEN];  /* buffer for single label */

    /* search for entry labels in labels table */
    while (label != NULL) {
        /* check if entry label */
        if (label->type == ENTRY) {
            /* allocate memory for converted base 4 address */
            if ((base4_address = (char *)malloc(MAX_DIGITS_BASE_4 + 1)) == NULL) {
                fprintf(stderr, "Memory allocation error for base 4 address\n");
                return STATUS_CODE_ERR;
            }

            decimal_to_base4(label->address, base4_address);
            /* add entry line */
            sprintf(line, "%s\t%s\n", label->name, base4_address);
            strcat(entry_content, line);  /* append to entry content */

            free(base4_address);  /* free allocated memory */
        }
        /* move to next label */
        label = label->next;
    }

    /* write content to file */
    return write_to_file(filename, entry_content, ENTRIES_FILE_EXTEN);
}