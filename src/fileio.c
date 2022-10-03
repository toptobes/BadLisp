#include <stdio.h>
#include <malloc.h>
#include "fileio.h"

char *readcode(const char *file_path_in)
{
    FILE *file = fopen(file_path_in, "r");

    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    rewind(file);

    char *buffer = calloc(size, sizeof(*buffer));
    fread(buffer, 1, size, file);

    fclose(file);

    return buffer;
}

void writecode(const char *file_path_out, const char *code)
{
    FILE *file = fopen(file_path_out, "w");
    fputs(code, file);
    fclose(file);
}
