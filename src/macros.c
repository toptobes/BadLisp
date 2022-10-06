#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "dynamic_string.h"
#include "util.h"
#include "macros.h"

Macro* create_macro(List* list, int* error)
{
    Macro *ds = malloc(sizeof(*ds));

    ds->name = list->args[0].as_word;
    ds->format = list->args[1].as_word;

    ds->delimiter = (list->args[2].as_word != NULL)
                    ? list->args[2].as_word
                    : strdup("\"\"");
    return ds;
}

char* expand_macro(List* list, Macro* macro, int* error)
{
    DynamicString *formatted = dstr_new(2 << 7);
    char *fmt = macro->format;

    str_remove_surrounding_quotes(macro->delimiter);

    for (int i = 1; i < strlen(fmt) - 1; i++)
    {
        if (fmt[i - 1] == '%' && isdigit(fmt[i]))
        {
            dstr_drop_last(formatted, 1);
            dstr_cat(formatted, list->args[fmt[i] - '0'].as_word);
        }
        else if (fmt[i - 1] == '%' && fmt[i] == '\\')
        {
            continue;
        }
        else if (i >= 2 && fmt[i - 2] == '%' && fmt[i - 1] == 'r' && isdigit(fmt[i]))
        {
            dstr_drop_last(formatted, 2);

            for (int j = fmt[i] - '0'; j < list->argc; j++)
            {
                dstr_cat(formatted, list->args[j].as_word);

                if (j < list->argc - 1)
                {
                    dstr_cat(formatted, macro->delimiter);
                }
            }
        }
        else
        {
            dstr_append(formatted, fmt[i]);
        }
    }

    return dstr_destroy_wrapper(&formatted);
}

MacroList* macrolist_new(int size)
{
    MacroList *list = malloc(sizeof(*list));

    list->macros = malloc(size * sizeof(Macro*));
    list->backing_arr_size = size;
    list->length = 0;

    return list;
}

void macrolist_add(MacroList* self, Macro* macro)
{
    self->macros[self->length++] = macro;

    if (self->length >= self->backing_arr_size)
    {
        self->macros = realloc(self->macros, (self->backing_arr_size *= 2));
    }
}
