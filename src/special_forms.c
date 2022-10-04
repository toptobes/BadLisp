#include <string.h>
#include "util.h"
#include "dynamic_string.h"
#include "special_forms.h"

static char* expand_typed_var(char *var, int *error)
{
    char *name = strtok(var, "::");
    char *type = strtok(NULL, "::");

    DynamicString *declaration = dstr_new(2 << 5);

    int num_brackets = 0;

    if (type != NULL)
    {
        for (int i = 0; i < strlen(type); i++)
        {
            num_brackets += type[i] == '[';

            if (type[i] == '.' || type[i] == '[' || type[i] == ']')
            {
                type[i] = ' ';
            }
        }

        dstr_cat(declaration, type);
        dstr_append(declaration, ' ');
    }

    dstr_cat(declaration, name);

    for (int i = 0; i < num_brackets; i++)
    {
        dstr_cat(declaration, "[]");
    }

    return dstr_destroy_wrapper(&declaration);
}

char* expand_do(List* list, int *error)
{
    DynamicString *code = dstr_new(2<<8);
    dstr_append(code, '{');

    for (int i = 0; i < list->argc; i++)
    {
        dstr_cat(code, list->args[i].as_word);
        dstr_append(code, ';');
    }

    dstr_append(code, '}');
    return dstr_destroy_wrapper(&code);
}

char* expand_if(List* list, int* error)
{
    char *else_statement = (list->args[2].as_word == NULL) ? "" : list->args[2].as_word;
    return str_from_format("if(%s){%s;}else{%s;}", list->args[0].as_word, list->args[1].as_word, else_statement);
}

char* expand_def(List* list, int *error)
{
    char *var_declaration = expand_typed_var(list->args->as_word, error);

    if (!str_is_blank(list->args[1].as_word))
    {
        return str_from_format("%s=%s;", var_declaration, list->args[1].as_word);
    }

    return str_from_format("%s;", var_declaration);
}

char* expand_let(List* list, int *error)
{
    char *bindings_vector = (list->args[0].as_word + 1);
    bindings_vector[strlen(bindings_vector) - 1] = '\0';

    bool in_parentheses = false;
    for (int i = 0; i < strlen(bindings_vector); i++)
    {
        switch (bindings_vector[i])
        {
            case '(':
                in_parentheses = true;
                break;

            case ')':
                in_parentheses = false;
                break;

            case '\"':
                skip_string(bindings_vector, &i);
                break;

            case ',':
                if (!in_parentheses)
                {
                    bindings_vector[i] = '|';
                }
                break;
        }
    }

    char *binding_name, *binding_value;
    DynamicString *bindings = dstr_new(2 << 8);

    while ((binding_name = str_sep(&bindings_vector, "|")) != NULL)
    {
        binding_value = str_sep(&bindings_vector, "|");

        binding_name = expand_typed_var(binding_name, error);

        char *binding = str_from_format("%s = %s;", binding_name, binding_value);
        dstr_cat(bindings, binding);
    }


    DynamicString *body = dstr_new(2<<10);

    for (int i = 1; i < list->argc; i++)
    {
        dstr_cat(body, list->args[i].as_word);
        dstr_append(body, ';');
    }


    char *let_block = str_from_format("{%s%s}", bindings->str, body->str);

    dstr_destroy(&bindings);
    dstr_destroy(&body);

    return let_block;
}

char* expand_defn(List* list, int *error)
{
    char *func_declaration = expand_typed_var(list->args->as_word, error);


    char *args_vector = (list->args[1].as_word + 1);
    args_vector[strlen(args_vector) - 1] = '\0';

    char *arg;
    DynamicString *args = dstr_new(2<<8);

    while ((arg = str_sep(&args_vector, ",")))
    {
        dstr_cat(args, expand_typed_var(arg, error));
        dstr_append(args, ',');
    }

    dstr_drop_last(args, 1);


    char *function;
    if (list->args[2].as_word != NULL)
    {
        DynamicString *code_block = dstr_new(2<<10);

        for (int i = 2; i < list->argc; i++)
        {
            dstr_cat(code_block, list->args[i].as_word);
            dstr_append(code_block, ';');
        }

        function = str_from_format("%s(%s){%s}", func_declaration, args->str, code_block->str);

        dstr_destroy(&code_block);
    }
    else
    {
        function = str_from_format("%s(%s);", func_declaration, args->str);
    }

    dstr_destroy(&args);

    return function;
}

char* expand_quote(List* list, int *error)
{
    DynamicString *code = dstr_new(2<<8);
    dstr_append(code, '(');

    for (int i = 0; i < list->argc; i++)
    {
        dstr_cat(code, list->args[i].as_word);
    }

    dstr_append(code, ')');
    return dstr_destroy_wrapper(&code);
}

char* expand_defarr(List* list, int *error)
{
    char *arr_declaration = expand_typed_var(list->args[0].as_word, error);
    char *arr_size = list->args[1].as_word;
    return str_from_format("%s[%s];", arr_declaration, arr_size);
}

char* expand_vector(List* list, int *error)
{
    DynamicString *code = dstr_new(2<<8);
    dstr_append(code, '{');

    for (int i = 0; i < list->argc; i++)
    {
        dstr_cat(code, list->args[i].as_word);

        if (i < list->argc - 1)
        {
            dstr_append(code, ',');
        }
    }

    dstr_append(code, '}');
    return dstr_destroy_wrapper(&code);
}

char* expand_global_wrapper(List* list, int *error)
{
    DynamicString *code = dstr_new(2<<10);

    for (int i = 0; i < list->argc; i++)
    {
        dstr_cat(code, list->args[i].as_word);
    }

    return dstr_destroy_wrapper(&code);
}

char* expand_c_preprocessor_command(List *list, int *error)
{
    DynamicString *code = dstr_new(2<<8);

    dstr_append(code, '#');
    dstr_cat(code, list->args->as_word);

    for (int i = 1; i < list->argc; i++)
    {
        dstr_append(code, ' ');
        dstr_cat(code, list->args[i].as_word);
    }

    dstr_append(code, '\n');
    return dstr_destroy_wrapper(&code);
}

char* expand_operator(List *list, int *error)
{
    DynamicString *operation = dstr_new(2 << 4);

    dstr_append(operation, '(');
    dstr_cat(operation, list->args[0].as_word);

    for (int i = 1; i < list->argc; i++)
    {
        dstr_cat(operation, list->func_name);
        dstr_cat(operation, list->args[i].as_word);
    }

    dstr_append(operation, ')');

    return dstr_destroy_wrapper(&operation);
}
