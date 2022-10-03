#include <stdio.h>
#include <unistd.h>
#include "../../src/bad_lisp.h"
#include "../../src/util.h"

int main(void)
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    char *test1_file_in = str_from_format("%s%s", cwd, "\\playground\\playground.bl");
    char *test1_file_out = str_from_format("%s%s", cwd, "\\playground\\out\\playground.bl.c");

    transpile_file(test1_file_in, test1_file_out, NULL);
    return 0;
}