#ifndef BAD_LISP_MACROS_H
#define BAD_LISP_MACROS_H

#include "list.h"

typedef struct {
    char* name;
    char* format;
    char* delimiter;
} Macro;

typedef struct {
    size_t  length;
    size_t  backing_arr_size;
    Macro** macros;
} MacroList;

Macro*  create_macro(List *list, int *error);
char*   expand_macro(List*, Macro*, int*);

MacroList*  macrolist_new(int);
void        macrolist_add(MacroList*, Macro*);

#endif
