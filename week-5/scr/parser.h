#ifndef PARSER_H
#define PARSER_H

#include "simulator.h"

/* Returns 1 on success. Accepts spaces, commas, and tabs as separators. */
int parse_references(const char *input, int output[], int *count,
                     char *error, int error_size);

#endif