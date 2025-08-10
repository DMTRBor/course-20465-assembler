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


void word_to_base4(Word encoded_value, char *converted) {
    unsigned int word_decimal;
    
    /* combine all Word fields into a single 10-bit value */
    word_decimal = (encoded_value.opcode << 6) |
                   (encoded_value.src_operand << 4) |
                   (encoded_value.dest_operand << 2) |
                    encoded_value.encoding_type;
    
    /* convert decimal word to base4 */
    decimal_to_base4(word_decimal, converted);

    /* pad with leading zeros ('a') for 5-digit representation */
    while (strlen(converted) < MAX_DIGITS_BASE_4) {
        memmove(converted + 1, converted, strlen(converted) + 1);  /* handle the buffer overlap */
        converted[0] = base4_encoded[0];
    }
}


int code_and_data_size_base4_to_file(unsigned int ICF, unsigned int DCF, char *line, char *content) {
    char *base4_address;  /* buffer for base 4 address */

    /* allocate memory for converted base 4 address */
    if ((base4_address = (char *)malloc(MAX_DIGITS_BASE_4 + 1)) == NULL) {
        fprintf(stderr, "Memory allocation error for base 4 address\n");
        return STATUS_CODE_ERR;
    }

    decimal_to_base4(ICF - IC_RESET_VALUE, base4_address);  /* convert code size to base 4 */

    sprintf(line, "%s\t", base4_address);
    strcat(content, line);  /* append code size value to content */

    decimal_to_base4(DCF, base4_address);  /* convert data size to base 4 */

    sprintf(line, "%s\n", base4_address);
    strcat(content, line);  /* append data size value to content */

    free(base4_address);  /* free allocated memory */
    return STATUS_CODE_OK;  /* success */
}


int write_full_content_to_file(char *filename, char *content, char *extension) {
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


int build_object_file(char *filename, unsigned int ICF, unsigned int DCF, MemoryUnit **mem_map) {
    MemoryUnit *mem_unit = *mem_map;

    char *base4_address, *base4_code;  /* buffer for base 4 address */

    char obj_content[MAX_FILE_BUFFER] = "";  /* buffer to store memory map */
    char line[MAX_LINE_LEN];  /* buffer for memory unit content */

    /* convert ICF and DCF to base 4 and add to file */
    if (code_and_data_size_base4_to_file(ICF, DCF, line, obj_content) == STATUS_CODE_ERR) {
        fprintf(stderr, "Error converting code and data size to base 4\n");
        return STATUS_CODE_ERR;
    }

    /* add content from memory table to object file buffer */
    while (mem_unit != NULL) {
        /* allocate memory for converted base 4 address */
        if ((base4_address = (char *)malloc(MAX_DIGITS_BASE_4 + 1)) == NULL) {
            fprintf(stderr, "Memory allocation error for base 4 address\n");
            return STATUS_CODE_ERR;
        }
        /* allocate memory for converted base 4 code */
        if ((base4_code = (char *)malloc(MAX_DIGITS_BASE_4 + 1)) == NULL) {
            fprintf(stderr, "Memory allocation error for base 4 code\n");
            return STATUS_CODE_ERR;
        }

        decimal_to_base4(mem_unit->address, base4_address);  /* address to base 4 */
        word_to_base4(mem_unit->encoded_value, base4_code);  /* code to base 4 */

        sprintf(line, "%s\t%s\n", base4_address, base4_code);
        strcat(obj_content, line);  /* append to object content */

        free(base4_address);  /* free allocated memory */
        free(base4_code);  /* free allocated memory */

        /* move to next memory unit */
        mem_unit = mem_unit->next;
    }

    /* write content to file */
    return write_full_content_to_file(filename, obj_content, OBJECT_FILE_EXTEN);
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

            decimal_to_base4(mem_unit->address, base4_address);  /* address to base 4 */
            /* add external line */
            sprintf(line, "%s\t%s\n", mem_unit->label_op_name, base4_address);
            strcat(ext_content, line);  /* append to external content */

            free(base4_address);  /* free allocated memory */
        }
        /* move to next memory unit */
        mem_unit = mem_unit->next;
    }

    /* write content to file */
    return write_full_content_to_file(filename, ext_content, EXTERNAL_FILE_EXTEN);
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

            decimal_to_base4(label->address, base4_address);  /* address to base 4 */
            /* add entry line */
            sprintf(line, "%s\t%s\n", label->name, base4_address);
            strcat(entry_content, line);  /* append to entry content */

            free(base4_address);  /* free allocated memory */
        }
        /* move to next label */
        label = label->next;
    }

    /* write content to file */
    return write_full_content_to_file(filename, entry_content, ENTRIES_FILE_EXTEN);
}