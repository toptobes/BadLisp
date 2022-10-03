#ifndef PLAYGROUND_UTIL_H
#define PLAYGROUND_UTIL_H

char* str_from_format(const char*, ...);
int str_is_blank(const char*);
char *str_sep(char**, const char*);
void skip_string(const char*, int*);

#endif