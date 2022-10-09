#include <string.h>
#include <malloc.h>
#include "util/util.h"
#include "util/dynamic_string.h"
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

char* expand_if(const List *list, int *error)
{
    char *else_statement = (list->rest[2].as.word == NULL) ? "" : list->rest[2].as.word;
    return str_from_format("if(%s){%s;}else{%s;}", list->rest[0].as.word, list->rest[1].as.word, else_statement);
}

char* expand_def(const List *list, int *error)
{
    char *var_declaration = expand_typed_var(list->rest->as.word, error);

    if (!str_is_blank(list->rest[1].as.word))
    {
        return str_from_format("%s=%s;", var_declaration, list->rest[1].as.word);
    }

    return str_from_format("%s;", var_declaration);
}

char* expand_let(const List *list, int *error)
{
    char *bindings_vector = (list->rest[0].as.word + 1);
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

    for (int i = 1; i < list->rest_c; i++)
    {
        dstr_cat(body, list->rest[i].as.word);
        dstr_append(body, ';');
    }


    char *let_block = str_from_format("{%s%s}", bindings->str, body->str);

    dstr_destroy(&bindings);
    dstr_destroy(&body);

    return let_block;
}

char* expand_defn(const List *list, int *error)
{
    char *func_declaration = expand_typed_var(list->rest->as.word, error);


    char *args_vector = (list->rest[1].as.word + 1);
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
    if (list->rest[2].as.word != NULL)
    {
        DynamicString *code_block = dstr_new(2<<10);

        for (int i = 2; i < list->rest_c; i++)
        {
            dstr_cat(code_block, list->rest[i].as.word);
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

char* expand_quote(const List *list, int *error)
{
    DynamicString *code = dstr_new(2<<8);
    dstr_append(code, '(');

    for (int i = 0; i < list->rest_c; i++)
    {
        dstr_cat(code, list->rest[i].as.word);
    }

    dstr_append(code, ')');
    return dstr_destroy_wrapper(&code);
}

char* expand_vector(const List *list, int *error)
{
    DynamicString *code = dstr_new(2<<8);
    dstr_append(code, '{');

    for (int i = 0; i < list->rest_c; i++)
    {
        dstr_cat(code, list->rest[i].as.word);

        if (i < list->rest_c - 1)
        {
            dstr_append(code, ',');
        }
    }

    dstr_append(code, '}');
    return dstr_destroy_wrapper(&code);
}

char* expand_defmacro(const List *list, int *error)
{
    return calloc(1, 1);
}

char* expand_global_wrapper(const List *list, int *error)
{
    DynamicString *code = dstr_new(2<<10);

    for (int i = 0; i < list->rest_c; i++)
    {
        dstr_cat(code, list->rest[i].as.word);
    }

    return dstr_destroy_wrapper(&code);
}

char* expand_c_preprocessor_command(const List *list, int *error)
{
    DynamicString *code = dstr_new(2<<8);

    dstr_append(code, '#');
    dstr_cat(code, list->rest->as.word);

    for (int i = 1; i < list->rest_c; i++)
    {
        dstr_append(code, ' ');
        dstr_cat(code, list->rest[i].as.word);
    }

    dstr_append(code, '\n');
    return dstr_destroy_wrapper(&code);
}

char* expand_operator(const List *list, int *error)
{
    DynamicString *operation = dstr_new(2 << 4);

    dstr_append(operation, '(');
    dstr_cat(operation, list->rest[0].as.word);

    for (int i = 1; i < list->rest_c; i++)
    {
        dstr_cat(operation, list->head);
        dstr_cat(operation, list->rest[i].as.word);
    }

    dstr_append(operation, ')');

    return dstr_destroy_wrapper(&operation);
}
