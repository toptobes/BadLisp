#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "dynamic_string.h"
#include "macros.h"

Macro* expand_defmacro(List* list, int* error)
{
    Macro *ds = malloc(sizeof(*ds));

    ds->name = list->args[0].as_word;
    ds->format = list->args[1].as_word;
    ds->delimiter = list->args[2].as_word;

    return ds;
}

char* expand_macro(List* list, Macro* macro, int* error)
{
    DynamicString *format = dstr_new(2 << 5);

    for (int i = 0; i < strlen(macro->format) - 1; i++)
    {
        dstr_append(format, code[i]);

        if (macro->format[i] != '\\' && macro->format[i + 1] == '\\')
            i++;
    }

    DynamicString *formatted = dstr_new(2 << 7);

    for (int i = 1; i < format->length; i++)
    {
        if (format->str[i - 1] == '%' && isdigit(format->str[i]))
        {
            dstr_drop_last(formatted, 1);
        }
        else
        {
            dstr_append()
        }
    }
}