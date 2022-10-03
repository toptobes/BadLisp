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

static char* extract_string(const char*, int*);

char* preprocess(const char *code, int *error)
{
    DynamicString *new_code = dstr_new(2<<10);
    dstr_cpy(new_code, "(^root ");

    bool is_array_declaration = false;

    for (int i = 0; i < strlen(code); i++)
    {
        is_array_declaration =
                (i > 0) && (is_array_declaration || ((code[i] == '[') && (code[i - 1] != ')') && (code[i - 1] != ' ')));

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

            case '[':
                dstr_cat(new_code, is_array_declaration ? "[" : "(vector ");
                break;

            case ']':
                dstr_append(new_code, is_array_declaration ? ']' : ')');
                is_array_declaration = false;
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

List* tokenize(char *code, int *error)
{
    ListpStack *stack = Listpstack_create(2<<8);
    List *current_list = list_new();

    bool is_func_name = false;
    DynamicString *current_word = dstr_new(2<<8);

    char c;
    for (int i = 0; i < strlen(code); i++)
    {
        c = code[i];

        if (c == '\"')
        {
            dstr_cpy(current_word, extract_string(code, &i));
        }

        else if (c == '(' || c == ' ' || c == ',' || c == ')')
        {
            if (!str_is_blank(current_word->str))
            {
                list_add_string(current_list, current_word->str, is_func_name);
                dstr_clear(current_word);
                is_func_name = false;
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
                is_func_name = true;
            }
        }

        else
        {
            dstr_append(current_word, c);
        }
    }

    Listpstack_complete_destroy(stack);

    List* tokenized_code = current_list->args->as_list;
    free(current_list);

    return tokenized_code;
}

static char* extract_string(const char* code, int* i)
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

char* expand(List* list, int *error)
{
    for (int i = 0; i < list->argc; i++)
    {
        if (list->args[i].type == LIST)
        {
            list->args[i].as_word = expand(list->args[i].as_list, error);
            list->args[i].type = WORD;
        }
    }

    for (int i = 0; i < NUM_SPECIAL_FORMS; i++)
    {
        bool is_special_form = strcmp(special_forms[i].form, list->func_name) == 0;

        if (is_special_form)
        {
            return special_forms[i].expander(list, error);
        }
    }

    return expand_default(list, error);
}

char *expand_default(List *list, int *error)
{
    DynamicString *code = dstr_new_copy(list->func_name);
    dstr_append(code, '(');

    for (int i = 0; i < list->argc; i++)
    {
        dstr_cat(code, list->args[i].as_word);

        if (i < list->argc - 1)
        {
            dstr_append(code, ',');
        }
    }

    dstr_append(code, ')');
    return dstr_destroy_wrapper(&code);
}
