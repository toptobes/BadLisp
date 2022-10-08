#include <malloc.h>
#include "macros.h"

MacroList* macrolist_new(int size)
{
    MacroList *list = malloc(sizeof(*list));

    list->macros = malloc(size * sizeof(Macro*));
    list->backing_arr_size = size;
    list->length = 0;

    return list;
}

void macrolist_add(MacroList *self, Macro *macro)
{
    self->macros[self->length++] = macro;

    if (self->length >= self->backing_arr_size)
    {
        self->macros = realloc(self->macros, (self->backing_arr_size *= 2));
    }
}
