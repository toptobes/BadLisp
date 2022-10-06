#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "util.h"

char* str_from_format(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    int length = vsnprintf(NULL, 0, format, args);

    char *string = malloc(length + 1);

    vsprintf(string, format, args);

    va_end(args);

    return string;
}

int str_is_blank(const char *str)
{
    if (str == NULL)
        return true;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] != ' ' && str[i] != '\t')
            return false;
    }
    return true;
}

char *str_sep(char **stringp, const char *delim)
{
    if (*stringp == NULL)
    {
        return NULL;
    }

    char *token_start = *stringp;

    *stringp = strpbrk(token_start, delim);

    if (*stringp)
    {
        **stringp = '\0';
        (*stringp)++;
    }

    return token_start;
}

void skip_string(const char *code, int *i)
{
    bool is_escaped = true;

    for (int j = 0; code[*i] != '\"' || is_escaped; j++)
    {
        is_escaped = (code[(*i)++] == '\\') && !is_escaped;
    }
}

void str_remove_surrounding_quotes(char *code)
{
    if (code[strlen(code) - 1] == '"')
    {
        code[strlen(code) - 1] = '\0';
    }

    if (code[0] == '"')
    {
        memmove(code, code + 1, strlen(code));
    }
}