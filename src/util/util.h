#ifndef PLAYGROUND_UTIL_H
#define PLAYGROUND_UTIL_H

#include <stdbool.h>

char* str_from_format(const char *format, ...);
int   str_is_blank(const char *str);
char* str_sep(char **str_ptr, const char *delim);
void  skip_string(const char *code, int *i);
bool  char_in(char c, const char *chars);
char* expand_var_declaration(const char *code, int *i, int *error);
int   str_index_of_char(const char* str, char c);

#endif