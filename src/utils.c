#include "../hdr/utils.h"


/**
 * This function receives a decimal number string
 * representation and an empty string, converts the
 * decimal number to base 4 representation
 * and places the converted string representation of
 * the number in base 4 in an empty string.
 */
void decimal_to_base4(char *decimal, char *converted) {
    /* temporary converted number storage */
    char temp[sizeof(converted) / sizeof(char)];
    int decimal_num = 0;
    int power = 2;  /* hundreds */
    int i = 0, j;

    /* skip first zero, address starts from 100 */
    decimal++;
    
    /* convert string to integer */
    while (*decimal != NULL_TERMINATOR) {
        decimal_num += (*decimal - ZERO) * pow(BASE_10, power);
        decimal++;
        power--;
    }
    
    /* store reversed conversion of decimal to base 4 */
    while (decimal_num > 0) {
        temp[i++] = (decimal_num % BASE_4) + ZERO;
        decimal_num /= BASE_4;
    }
    temp[i] = NULL_TERMINATOR;
    
    /* revert the number in base 4, skip null terminator */
    for (j = i - 1; j >= 0; j--) {
        *converted = temp[j];
        converted++;
    }
    *converted = NULL_TERMINATOR;
}


/**
 * This function receives a data
 * stream read from a file and
 * returns the size of the stream/
 * content in bytes.
 */
long file_content_size(FILE* fp) {
    long content_size;

    fseek(fp, 0, SEEK_END);
    content_size = ftell(fp);
    rewind(fp);  /* return to start of the file */

    return content_size;
}


/**
 * This function receives a string
 * that represents a filename, opens
 * the file for reading and returns a
 * pointer to file if succeeded in
 * reading the content. If failed to
 * read from file (not exitent, permission),
 * the function returns null.
 */
FILE* open_file(char *filename, char *permission) {
    FILE *fp = fopen(filename, permission);

    if (fp == NULL) {
        fprintf(stderr, "Can't open file - %s\n", filename);
        fclose(fp);
        return NULL;
    }

    return fp;
}


/**
 * This function gets a pointer to a
 * line read from file, that has a
 * pre-defined maximum length and a
 * pointer to a file stream and checks
 * if the line is too long (if newline
 * is not found in characters, the line
 * is longer than expected). The function
 * returns TRUE code if the line is too
 * long and FALSE code otherwise.
 */
int line_too_long(char *line, FILE *fp) {
    /* count line characters until
       newline (if exists) */
    size_t line_length = strcspn(line, NEWLINE_STR);

    /* newline not found at the end of the
       line and not reached EOF */
    return (line[line_length] != NEWLINE_CHAR && !feof(fp));
}