#include <string.h>
#include <malloc.h>
#include "transpile.h"
#include "util/fileio.h"
#include "util/util.h"
#include "bad_lisp.h"

static char* get_file_name(const char *str);

char* transpile_string(const char* code, int* error)
{
    char *processed_code = preprocess(code, error);
    List *tokenized_code = tokenize(processed_code, error);
    char *transpiled_code = expand_list(tokenized_code, error);

    return transpiled_code;
}

char* transpile_file(const char* file_path_in, const char* output_dir, int* error)
{
    char *code = readcode(file_path_in);
    char *transpiled_code = transpile_string(code, error);

    char *file_name = get_file_name(file_path_in);
    char *file_path_out = str_from_format("%s%s.c", output_dir, file_name);

    writecode(file_path_out, transpiled_code);

    return transpiled_code;
}

static char* get_file_name(const char *str)
{
    const char *last = strrchr(str, '\\');

    if (last != NULL)
    {
        const size_t len = strlen(last);
        char *new = malloc(len + 1);

        memcpy(new, last, len);
        new[len] = '\0';

        return new;
    }

    return NULL;
}
