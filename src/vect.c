#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "list.h"
#include "vect.h"

Vect* vect_new()
{
    Vect *vect = malloc(sizeof(*vect));
    vect->type = NULL;
    vect->elems_c = 0;
    vect->elems = malloc(10 * sizeof(ListElement));
    vect->backing_arr_size = 10;
    return vect;
}

static void indent(int num_tabs)
{
    for (int i = 0; i < num_tabs; i++)
    {
        fputs("  ", stdout);
    }
}

void vect_print(const Vect *self, int depth)
{
    indent(depth-1);
    puts("[");

    indent(depth);
    if (self->type != NULL)
    {
        printf("Type: %s", self->type);
    }

    for (int i = 0; i < self->elems_c; i++)
    {
        if (self->elems[i].type == WORD)
        {
            indent(depth);
            puts(self->elems[i].as.word);
        }
        else if (self->elems[i].type == LIST)
        {
            list_print(self->elems[i].as.list, depth + 1);
        }
        else
        {
            vect_print(self->elems[i].as.vect, depth + 1);
        }
    }

    indent(depth-1);
    puts("]");
}

void vect_set_type(Vect *self, const char* type)
{
    self->type = strdup(type);
}

void vect_add_word(Vect *self, const char *word)
{
    self->elems[self->elems_c].type = WORD;
    self->elems[self->elems_c++].as.word = strdup(word);
}

void vect_add_list(Vect *self, List *list)
{
    self->elems[self->elems_c].type = LIST;
    self->elems[self->elems_c++].as.list = list;
}

void vect_add_vect(Vect *self, Vect *nested_vect)
{
    self->elems[self->elems_c].type = VECT;
    self->elems[self->elems_c++].as.list = nested_vect;
}
