#include <stdio.h>
#include <unistd.h>
#include "../src/bad_lisp.h"
#include "../src/util.h"

const char *PLAYGROUND_FILE_PATH_IN  = "C:\\Users\\wanna\\Documents\\GitHub\\my-other-lisp\\test\\playground.bl";
const char *PLAYGROUND_FILE_PATH_OUT = "C:\\Users\\wanna\\Documents\\GitHub\\my-other-lisp\\test\\playground.bl.c";

int main(void)
{
    char *code = transpile_file(PLAYGROUND_FILE_PATH_IN, PLAYGROUND_FILE_PATH_OUT, NULL);
    puts(code);
    return 0;
}

int transpile_tests(void)
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    char *test1_file_in = str_from_format("%s%s", cwd, "\\test1.bl");
    char *test1_file_out = str_from_format("%s%s", cwd, "\\test1.bl.c");

    transpile_file(test1_file_in, test1_file_out, NULL);
    return 0;
}