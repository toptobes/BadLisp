#ifndef BAD_LISP_MACROS_H
#define BAD_LISP_MACROS_H

#include "list.h"

typedef struct {
    char *name;
    char *format;
    char *delimiter;
} Macro;

Macro* expand_defmacro(List*, int*);
char* expand_macro(List*, Macro*, int*);

#endif
