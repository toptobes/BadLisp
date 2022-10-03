#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "list.h"

List* list_new()
{
    List *list = malloc(sizeof(*list));
    list->argc = 0;
    list->args = malloc(10 * sizeof(ListElement));
    return list;
}

static void indent(int num_tabs)
{
    for (int i = 0; i < num_tabs; i++)
    {
        fputs("  ", stdout);
    }
}

void list_print(List* list, int depth)
{
    indent(depth-1);
    puts("(");

    indent(depth);
    puts(list->func_name);

    for (int i = 0; i < list->argc; i++)
    {
        if (list->args[i].type == WORD)
        {
            indent(depth);
            puts(list->args[i].as_word);
        }
        else
        {
            list_print(list->args[i].as_list, depth + 1);
        }
    }

    indent(depth-1);
    puts(")");
}

void list_add_string(List *list, const char *string, bool is_func_name)
{
    char *word = strdup(string);

    if (is_func_name)
    {
        list->func_name = word;
    }
    else
    {
        list->args[list->argc].type = WORD;
        list->args[list->argc].as_word = word;
        list->argc++;
    }
}

void list_add_list(List *list, List *list_to_nest)
{
    list->args[list->argc].type = LIST;
    list->args[list->argc++].as_list = list_to_nest;
}
