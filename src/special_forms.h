#ifndef BAD_LISP_SPECIAL_FORMS_H
#define BAD_LISP_SPECIAL_FORMS_H

#include "datastructures/list.h"

#define DEF_SPECIAL_FORM(name) char* expand_##name(const List *list, int *error) // NOLINT(bugprone-macro-parentheses)

DEF_SPECIAL_FORM(   do                       );
DEF_SPECIAL_FORM(   if                       );
DEF_SPECIAL_FORM(   let                      );
DEF_SPECIAL_FORM(   def                      );
DEF_SPECIAL_FORM(   defn                     );
DEF_SPECIAL_FORM(   quote                    );
DEF_SPECIAL_FORM(   operator                 );
DEF_SPECIAL_FORM(   global_wrapper           );
DEF_SPECIAL_FORM(   c_preprocessor_command   );

#undef DEF_SPECIAL_FORM

typedef struct {
    char* name;
    char* (*expander)(const List*, int*);
} SpecialForm;

static const SpecialForm special_forms[] = {
    { .name = "if",       .expander = expand_if},
    { .name = "def",      .expander = expand_def},
    { .name = "let",      .expander = expand_let},
    { .name = "defn",     .expander = expand_defn},
    { .name = "quote",    .expander = expand_quote},
    { .name = "^root",    .expander = expand_global_wrapper},
    { .name = "#",        .expander = expand_c_preprocessor_command},
    { .name = "+",        .expander = expand_operator},
    { .name = "-",        .expander = expand_operator},
    { .name = "*",        .expander = expand_operator},
    { .name = "/",        .expander = expand_operator},
    { .name = "<",        .expander = expand_operator},
    { .name = ">",        .expander = expand_operator},
    { .name = "<=",       .expander = expand_operator},
    { .name = ">=",       .expander = expand_operator},
    { .name = "<<",       .expander = expand_operator},
    { .name = ">>",       .expander = expand_operator},
    { .name = "&&",       .expander = expand_operator},
    { .name = "||",       .expander = expand_operator},
    { .name = "^",        .expander = expand_operator},
    { .name = "==",       .expander = expand_operator},
    { .name = "!=",       .expander = expand_operator},
};

static const int NUM_SPECIAL_FORMS = sizeof(special_forms) / sizeof(*special_forms);

#endif //BAD_LISP_SPECIAL_FORMS_H
