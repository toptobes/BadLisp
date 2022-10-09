#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "util/dynamic_string.h"
#include "special_forms.h"
#include "util/util.h"
#include "transpile.h"

typedef List *Listp;
typedef Vect *Vectp;

#define STACK_TYPE Listp
#include "util/stack.h"
#undef STACK_TYPE

#define STACK_TYPE Vectp
#include "util/stack.h"
#undef STACK_TYPE

static char* extract_string(const char *code, int *i);

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

List* tokenize(const char *code, int *error)
{
    int i = 0;
    return tokenize_list(code, &i, error);
}

char* expand_list(const List *list, int *error)
{
    for (int i = 0; i < NUM_SPECIAL_FORMS; i++)
    {
        bool is_special_form = strcmp(special_forms[i].name, list->head) == 0;

        if (is_special_form)
        {
            return special_forms[i].expander(list, error);
        }
    }

    expand_non_words_default(list, 0, error);

    return expand_function(list, error);
}

char* expand_vect(const Vect *vector, int *error)
{
    expand_non_words_default((List*) vector, 0, error);

    DynamicString *array = dstr_new(10);

    if (vector->type != NULL)
    {
        dstr_cat(array, str_from_format("(%s)", vector->type));
    }

    dstr_append(array, '{');

    for (int i = 0; i < vector->elems_c; i++)
    {
        dstr_cat(array, vector->elems[i].as.word);

        if (i < vector->elems_c - 1)
        {
            dstr_append(array, ',');
        }
    }

    dstr_append(array, '}');
    return dstr_destroy_wrapper(&array);
}

char* expand_non_words_default(const List *list, int from_index, int *error)
{
    for (int i = from_index; i < list->rest_c; i++)
    {
        if (list->rest[i].type == LIST)
        {
            list->rest[i].as.word = expand_list(list->rest[i].as.list, error);
        }
        if (list->rest[i].type == VECT)
        {
            list->rest[i].as.word = expand_vect(list->rest[i].as.vect, error);
        }
    }
}

char* expand_function(const List *list, int *error)
{
    DynamicString *func = dstr_new_copy(list->head);
    dstr_append(func, '(');

    for (int i = 0; i < list->rest_c; i++)
    {
        dstr_cat(func, list->rest[i].as.word);

        if (i < list->rest_c - 1)
        {
            dstr_append(func, ',');
        }
    }

    dstr_append(func, ')');
    return dstr_destroy_wrapper(&func);
}

List* tokenize_list(const char *code, int *i, int *error)
{
    List* current_list = list_new();
    DynamicString *current_word = dstr_new(2<<5);

    bool is_arr_declaration = false;
    bool is_head = true;

    while (1)
    {
        char c = code[++(*i)];

        is_arr_declaration = is_arr_declaration || c == ':';

        if (c == '\"')
        {
            dstr_cpy(current_word, extract_string(code, i));
        }
        else if (c == '(')
        {
            List *nested_list = tokenize_list(code, i, error);
            list_add_list(current_list, nested_list);
        }
        else if (c == '[' && !is_arr_declaration)
        {
            Vect *nested_vect = tokenize_vect(code, i, error);
            list_add_vect(current_list, nested_vect);
        }
        else if (c == ']')
        {
            is_arr_declaration = false;
        }
        else if (c == ' ' || c == ',' || c == ')')
        {
            if (!str_is_blank(current_word->str))
            {
                list_add_word(current_list, current_word->str, is_head);
                dstr_clear(current_word);
                is_head = false;
            }

            if (c == ')')
            {
                break;
            }
        }
        else
        {
            dstr_append(current_word, c);
        }
    }

    return current_list;
}

static Vect* tokenize_vect(const char *code, int *i, int *error)
{
    Vect* current_vect = vect_new();
    DynamicString *current_word = dstr_new(2<<5);

    bool is_arr_declaration = false;

    while (1)
    {
        char c = code[++(*i)];

        is_arr_declaration = is_arr_declaration || c == ':';

        if (c == '\"')
        {
            dstr_cpy(current_word, extract_string(code, i));
        }
        else if (c == '(')
        {
            List *nested_list = tokenize_list(code, i, error);
            vect_add_list(current_vect, nested_list);
        }
        else if (c == '[' && !is_arr_declaration)
        {
            Vect *nested_vect = tokenize_vect(code, i, error);
            vect_add_vect(current_vect, nested_vect);
        }
        else if (c == ' ' || c == ',' || c == ']')
        {
            if (!str_is_blank(current_word->str))
            {
                vect_add_word(current_vect, current_word->str);
                dstr_clear(current_word);
            }

            if (c == ']' && !is_arr_declaration)
            {
                break;
            }

            is_arr_declaration = false;
        }
        else
        {
            dstr_append(current_word, c);
        }
    }

    if (code[(*i) + 1] == ':')
    {
        (*i)++;
        char *type = expand_var_declaration(code, i, error);
        vect_set_type(current_vect, type);
    }

    return current_vect;
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

int main(void)
{
    int i = 0;

    List *list = tokenize_list("(let [a::[char] \"1\"] (puts a))", &i, NULL);
    char *code = expand_list(list, NULL);

    puts(code);

    return 0;
}