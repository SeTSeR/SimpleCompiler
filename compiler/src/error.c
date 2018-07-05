#include "error.h"

#include <stdio.h>
#include <stdlib.h>

void error(char* message) {
    fputs("There was an error during the compilation:", stderr);
    fputs(message, stderr);
    exit(EXIT_FAILURE);
}
