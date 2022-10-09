#ifndef PLAYGROUND_UTIL_H
#define PLAYGROUND_UTIL_H

#include <stdbool.h>

char* str_from_format(const char *format, ...);
int   str_is_blank(const char *str);
char* str_sep(char **str_ptr, const char *delim);
void  skip_string(const char *code, int *i);
void  str_remove_surrounding_quotes(char *code);
bool  char_in(char c, const char *chars);
bool  is_valid_name_char(char c);

#endif