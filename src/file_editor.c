#include "ncurses.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void
edit_file(char *file_to_edit)
{
    char *buffer = malloc(strlen(file_to_edit) + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory in 0x231\n");
        exit(EXIT_FAILURE);
    }
}
