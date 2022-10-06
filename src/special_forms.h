#ifndef BAD_LISP_SPECIAL_FORMS_H
#define BAD_LISP_SPECIAL_FORMS_H

#include "list.h"

char* expand_do(List*, int *);
char* expand_if(List*, int *);
char* expand_def(List*, int *);
char* expand_let(List*, int *);
char* expand_defn(List*, int *);
char* expand_quote(List*, int *);
char* expand_vector(List*, int *);
char* expand_defmacro(List *list, int *error);
char* expand_operator(List*, int *);
char* expand_global_wrapper(List*, int *);
char* expand_c_preprocessor_command(List*, int *);

typedef char *SpecialForm_t;

typedef struct {
    SpecialForm_t form;
    char* (*expander)(List*, int*);
} Special_Form_Expander;

static const Special_Form_Expander special_forms[] = {
    { .form = "if",       .expander = expand_if},
    { .form = "def",      .expander = expand_def},
    { .form = "let",      .expander = expand_let},
    { .form = "defn",     .expander = expand_defn},
    { .form = "quote",    .expander = expand_quote},
    { .form = "vector",   .expander = expand_vector},
    { .form = "defmacro", .expander = expand_defmacro},
    { .form = "^root",    .expander = expand_global_wrapper},
    { .form = "#",        .expander = expand_c_preprocessor_command},
    { .form = "+",        .expander = expand_operator},
    { .form = "-",        .expander = expand_operator},
    { .form = "*",        .expander = expand_operator},
    { .form = "/",        .expander = expand_operator},
    { .form = "<",        .expander = expand_operator},
    { .form = ">",        .expander = expand_operator},
    { .form = "<=",       .expander = expand_operator},
    { .form = ">=",       .expander = expand_operator},
    { .form = "<<",       .expander = expand_operator},
    { .form = ">>",       .expander = expand_operator},
    { .form = "&&",       .expander = expand_operator},
    { .form = "||",       .expander = expand_operator},
    { .form = "^",        .expander = expand_operator},
    { .form = "==",       .expander = expand_operator},
    { .form = "!=",       .expander = expand_operator},
};

static const int NUM_SPECIAL_FORMS = sizeof(special_forms) / sizeof(*special_forms);

#endif //BAD_LISP_SPECIAL_FORMS_H
