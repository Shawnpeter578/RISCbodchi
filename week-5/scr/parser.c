#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "parser.h"

int parse_references(const char *input, int output[], int *count,
                     char *error, int error_size) {
    const char *p = input;
    char *end;
    long value;
    int n = 0;

    if (!input || !output || !count) return 0;

    while (*p) {
        while (*p && (isspace((unsigned char)*p) || *p == ',')) ++p;
        if (!*p) break;

        if (*p == '-' || *p == '+') {
            /* Signed page numbers are deliberately rejected. */
            if (error && error_size > 0)
                snprintf(error, error_size, "Page numbers must be non-negative integers.");
            return 0;
        }
        if (!isdigit((unsigned char)*p)) {
            if (error && error_size > 0)
                snprintf(error, error_size, "Invalid character in page-reference string.");
            return 0;
        }

        value = strtol(p, &end, 10);
        if (end == p || value < 0 || value > 2147483647L) {
            if (error && error_size > 0)
                snprintf(error, error_size, "Page number is outside the supported range.");
            return 0;
        }
        if (n >= MAX_REFERENCES) {
            if (error && error_size > 0)
                snprintf(error, error_size, "Maximum 500 page references are allowed.");
            return 0;
        }

        output[n++] = (int)value;
        p = end;

        if (*p && !(isspace((unsigned char)*p) || *p == ',')) {
            if (error && error_size > 0)
                snprintf(error, error_size, "Separate page numbers using spaces or commas.");
            return 0;
        }
    }

    if (n == 0) {
        if (error && error_size > 0)
            snprintf(error, error_size, "Enter at least one page reference.");
        return 0;
    }

    *count = n;
    if (error && error_size > 0) error[0] = '\0';
    return 1;
}