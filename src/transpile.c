#include <stdbool.h>
#include <string.h>
#include "dynamic_string.h"
#include "special_forms.h"
#include "util.h"
#include "transpile.h"

typedef List *Listp;

#define STACK_TYPE Listp
#include "stack.h"
#undef STACK_TYPE

#define STACK_TYPE Vectp
#include "stack.h"
#undef STACK_TYPE

static char* extract_string(const char *code, int *i);
static Vect* extract_vector(const char *code, int *i);

char* preprocess(const char *code, int *error)
{
    DynamicString *new_code = dstr_new(2<<10);
    dstr_cpy(new_code, "(^root ");

    for (int i = 0; i < strlen(code); i++)
    {
        switch (code[i])
        {
            case '\"':
                dstr_cat(new_code, extract_string(code, &i));
                break;

            case '#':
                dstr_append(new_code, '#');
                dstr_append(new_code, ' ');
                break;

            case ';':
                while (code[++i] != '\n')
                    ;
                break;

            case '\'':
                if (code[i + 1] == '(' && code[i + 2] != '\'')
                {
                    dstr_cat(new_code, "(quote ");
                    i++;
                }
                else
                {
                    dstr_append(new_code, '\'');
                }
                break;

            case '\n':
                dstr_append(new_code, ' ');
                break;

            default:
                dstr_append(new_code, code[i]);
                break;
        }
    }

    dstr_append(new_code, ')');
    return dstr_destroy_wrapper(&new_code);
}

static List* tokenize_list(const char *code, int *i, int *error);
static Vect* tokenize_vect(const char *code, int *i, int *error);

List* tokenize_list(const char *code, int *i, int *error)
{
    char c;
    bool is_head = true;
    List* current_list = list_new();
    DynamicString *current_word = dstr_new(2<<5);

    while ((c = code[*i]) != ')')
    {
        if (c == '\"')
        {
            list_add_word(current_list, code, true);
        }
        else if (c == '(')
        {
            List *nested_list = tokenize_list(code, i, error);
            list_add_list(current_list, nested_list);
        }
        else if (c == ' ' || c == ',')
        {
            if (!str_is_blank(current_word->str))
            {
                list_add_word(current_list, current_word->str, is_head);
                dstr_clear(current_word);
                is_head = false;
            }
        }
        else
        {
            dstr_append(current_word, c);
        }
    }

    return current_list;
}

List* tokenize(const char *code, int *error)
{
    ListpStack *stack = Listpstack_create(2<<8);
    List *current_list = list_new();

    bool is_head = false;
    bool is_arr_declaration = false;

    DynamicString *current_word = dstr_new(2<<8);

    char c;
    for (int i = 0; i < strlen(code); i++)
    {
        c = code[i];

        is_arr_declaration = is_arr_declaration || c == ':';

        if (c == '\"')
        {
            dstr_cpy(current_word, extract_string(code, &i));
        }

        else if (c == '(' || c == ' ' || c == ',' || c == ')')
        {
            if (!str_is_blank(current_word->str))
            {
                list_add_word(current_list, current_word->str, is_head);
                dstr_clear(current_word);
                is_head = false;
            }

            if (c == ')')
            {
                list_add_list(Listpstack_peek(stack), current_list);
                current_list = Listpstack_pop(stack);
            }

            if (c == '(')
            {
                Listpstack_push(stack, current_list);
                current_list = list_new();
                is_head = true;
            }
        }

        else
        {
            dstr_append(current_word, c);
        }
    }

    Listpstack_complete_destroy(stack);

    List* tokenized_code = current_list->rest->as_list;
    free(current_list);

    return tokenized_code;
}

char* expand(const List *list, int *error)
{
    for (int i = 0; i < list->rest_c; i++)
    {
        if (list->rest[i].type == LIST)
        {
            list->rest[i].as_word = expand(list->rest[i].as_list, error);
            list->rest[i].type = WORD;
        }
    }

    for (int i = 0; i < NUM_SPECIAL_FORMS; i++)
    {
        bool is_special_form = strcmp(special_forms[i].name, list->head) == 0;

        if (is_special_form)
        {
            return special_forms[i].expander(list, error);
        }
    }

    return expand_function(list, error);
}

char* expand_function(const List *list, int *error)
{
    DynamicString *code = dstr_new_copy(list->head);
    dstr_append(code, '(');

    for (int i = 0; i < list->rest_c; i++)
    {
        dstr_cat(code, list->rest[i].as_word);

        if (i < list->rest_c - 1)
        {
            dstr_append(code, ',');
        }
    }

    dstr_append(code, ')');
    return dstr_destroy_wrapper(&code);
}

static char* extract_string(const char *code, int *i)
{
    DynamicString *string = dstr_new(2<<8);
    bool is_escaped = true;

    for (int j = 0; code[*i] != '\"' || is_escaped; j++)
    {
        is_escaped = (code[*i] == '\\') && !is_escaped;

        dstr_append(string, code[(*i)++]);
    }

    dstr_append(string, '"');

    return dstr_destroy_wrapper(&string);
}
