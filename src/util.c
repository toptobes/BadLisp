#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
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

char *str_sep(char **str_ptr, const char *delim)
{
    if (*str_ptr == NULL)
    {
        return NULL;
    }

    char *token_start = *str_ptr;

    *str_ptr = strpbrk(token_start, delim);

    if (*str_ptr)
    {
        **str_ptr = '\0';
        (*str_ptr)++;
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

bool char_in(char c, const char *chars)
{
    for (int i = 0; i < strlen(chars); i++)
    {
        if (c == chars[i])
        {
            return true;
        }
    }
    return false;
}

bool is_valid_name_char(char c)
{
    return isalnum(c) || char_in(c, "_-?!");
}