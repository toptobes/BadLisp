#include "transpile.h"
#include "fileio.h"
#include "bad_lisp.h"

char* transpile_string(const char* code, int* error)
{
    char *processed_code = preprocess(code, error);
    List *tokenized_code = tokenize(processed_code, error);
    char *transpiled_code = expand(tokenized_code, error);

    return transpiled_code;
}

char* transpile_file(const char* file_path_in, const char* file_path_out, int* error)
{
    char *code = readcode(file_path_in);
    char *transpiled_code = transpile_string(code, error);
    writecode(file_path_out, transpiled_code);

    return transpiled_code;
}
