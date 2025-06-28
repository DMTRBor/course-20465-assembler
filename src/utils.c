#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../hdr/utils.h"


/**
 * This function receives a decimal number string
 * representation and an empty string, converts the
 * decimal number to base 4 representation
 * and places the converted string representation of
 * the number in base 4 in an empty string.
 */
void decimal_to_base4(char* decimal, char* converted) {
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