#ifndef BAD_LISP_BAD_LISP_H
#define BAD_LISP_BAD_LISP_H

char* transpile_string(const char*, int*);
char* transpile_file(const char*, const char*, int*);

char** transpile_strings(const char**, int*);
void transpile_files(const char**, const char**, int*);

#endif