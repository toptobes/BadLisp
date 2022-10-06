#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "transpile.h"
#include "fileio.h"
#include "util.h"
#include "bad_lisp.h"

int main(void)
{
    char *output_dir   = "C:\\Users\\gupta_919456\\CLionProjects\\BadLisp\\test\\playground\\out";
    char *file_path_in = "C:\\Users\\gupta_919456\\CLionProjects\\BadLisp\\test\\playground\\macros.blm";

    transpile_file(file_path_in, output_dir, NULL);
    return 0;
}

static char* get_file_name(const char *str);

char* transpile_string(const char* code, int* error)
{
    char *processed_code = preprocess(code, error);
    List *tokenized_code = tokenize(processed_code, error);

    MacroList *macros = macrolist_new(10);
    reg_macros(tokenized_code, macros, error);

    char *transpiled_code = expand(tokenized_code, macros, error);

    return transpiled_code;
}

char* transpile_file(const char* file_path_in, const char* output_dir, int* error)
{
    char *code = readcode(file_path_in);
    char *transpiled_code = transpile_string(code, error);

    char *file_name = get_file_name(file_path_in);
    char *file_path_out = str_from_format("%s%s.c", output_dir, file_name);

    puts(file_name);

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
