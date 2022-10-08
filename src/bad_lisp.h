#ifndef BAD_LISP_BAD_LISP_H
#define BAD_LISP_BAD_LISP_H

char*   transpile_string(const char *code, int *error);
char*   transpile_file(const char *file_path_in, const char *output_dir, int *error);

char**  transpile_strings(const char **code_arr, int *error);
void    transpile_files(const char **file_paths_in, const char *output_dir, int *error);

#endif