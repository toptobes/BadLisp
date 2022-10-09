#include <stdio.h>
#include <unistd.h>
#include "../src/bad_lisp.h"
#include "../src/util/util.h"

int main(void)
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    char *test1_file_in = str_from_format("%s%s", cwd, "\\tests\\test1.bl");
    char *output_dir = str_from_format("%s%s", cwd, "\\tests\\out");

    transpile_file(test1_file_in, output_dir, NULL);
    return 0;
}
