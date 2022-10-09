#include <string.h>
#include <malloc.h>
#include "util/util.h"
#include "util/dynamic_string.h"
#include "special_forms.h"
#include "transpile.h"

char* expand_if(const List *list, int *error)
{
    expand_non_words_default(list, 0, error);

    char *else_statement = (list->rest[2].as.word == NULL) ? "" : list->rest[2].as.word;
    return str_from_format("if(%s){%s;}else{%s;}", list->rest[0].as.word, list->rest[1].as.word, else_statement);
}

char* expand_def(const List *list, int *error)
{
    expand_non_words_default(list, 0, error);

    int i = str_index_of_char(list->rest[0].as.word, ':');
    char *var_declaration = expand_var_declaration(list->rest[0].as.word, &i, error);

    return str_from_format("%s=%s;", var_declaration, list->rest[1].as.word);
}

char* expand_let(const List *list, int *error)
{
    DynamicString *let_block = dstr_new(2 << 5);
    dstr_append(let_block, '{');

    Vect *bindings_vector = list->rest[0].as.vect;

    for (int i = 0; i < bindings_vector->elems_c; i += 2)
    {
        int j = str_index_of_char(bindings_vector->elems[0].as.word, ':');
        char *var_declaration = expand_var_declaration(bindings_vector->elems[0].as.word, &j, error);

        char *binding = str_from_format("%s=%s;", var_declaration, bindings_vector->elems[1].as.word);
        dstr_cat(let_block, binding);
    }

    expand_non_words_default(list, 1, error);

    for (int i = 1; i < list->rest_c; i++)
    {
        dstr_cat(let_block, list->rest[i].as.word);
        dstr_append(let_block, ';');
    }

    dstr_append(let_block, '}');
    return dstr_destroy_wrapper(&let_block);
}

char* expand_defn(const List *list, int *error)
{
    int i = str_index_of_char(list->rest[0].as.word, ':');
    char *func_declaration = expand_var_declaration(list->rest[0].as.word, &i, error);


    char *args_vector = (list->rest[1].as.word + 1);
    args_vector[strlen(args_vector) - 1] = '\0';

    char *arg;
    DynamicString *args = dstr_new(2<<8);

    while ((arg = str_sep(&args_vector, ",")))
    {
        int j = str_index_of_char(arg, ':');
        char *expanded_arg = expand_var_declaration(arg, &i, error);

        dstr_cat(args, expanded_arg);
        dstr_append(args, ',');
    }

    dstr_drop_last(args, 1);


    char *function;
    if (list->rest[2].as.word != NULL)
    {
        DynamicString *code_block = dstr_new(2<<10);

        for (int j = 2; j < list->rest_c; j++)
        {
            dstr_cat(code_block, list->rest[j].as.word);
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
    expand_non_words_default(list, 0, error);

    DynamicString *code = dstr_new(2<<8);
    dstr_append(code, '(');

    for (int i = 0; i < list->rest_c; i++)
    {
        dstr_cat(code, list->rest[i].as.word);
    }

    dstr_append(code, ')');
    return dstr_destroy_wrapper(&code);
}

char* expand_global_wrapper(const List *list, int *error)
{
    expand_non_words_default(list, 0, error);

    DynamicString *code = dstr_new(2<<10);

    for (int i = 0; i < list->rest_c; i++)
    {
        dstr_cat(code, list->rest[i].as.word);
    }

    return dstr_destroy_wrapper(&code);
}

char* expand_c_preprocessor_command(const List *list, int *error)
{
    expand_non_words_default(list, 0, error);

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
    expand_non_words_default(list, 0, error);

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
