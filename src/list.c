#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "list.h"

List* list_new()
{
    List *list = malloc(sizeof(*list));
    list->rest_c = 0;
    list->rest = malloc(10 * sizeof(ListElement));
    list->backing_arr_size = 10;
    return list;
}

static void indent(int num_tabs)
{
    for (int i = 0; i < num_tabs; i++)
    {
        fputs("  ", stdout);
    }
}

void list_print(const List* self, int depth)
{
    indent(depth-1);
    puts("(");

    indent(depth);
    puts(self->head);

    for (int i = 0; i < self->rest_c; i++)
    {
        if (self->rest[i].type == WORD)
        {
            indent(depth);
            puts(self->rest[i].as_word);
        }
        else
        {
            list_print(self->rest[i].as_list, depth + 1);
        }
    }

    indent(depth-1);
    puts(")");
}

void list_add_word(List *self, const char *word, bool is_func_name)
{
    char *word_cpy = strdup(word);

    if (is_func_name)
    {
        self->head = word_cpy;
    }
    else
    {
        self->rest[self->rest_c].type = WORD;
        self->rest[self->rest_c].as_word = word_cpy;
        self->rest_c++;
    }
}

void list_add_list(List *self, List *list_to_nest)
{
    self->rest[self->rest_c].type = LIST;
    self->rest[self->rest_c++].as_list = list_to_nest;
}

void list_add_vect(List *self, Vect *vector)
{
    self->rest[self->rest_c].type = VECT;
    self->rest[self->rest_c++].as_vect = vector;
}
