#ifndef BAD_LISP_MACROS_H
#define BAD_LISP_MACROS_H

#include "datastructures/list.h"

typedef struct {
    char* name;
    char* (*expander)(List*, int*);
} Macro;

typedef struct {
    size_t  length;
    size_t  backing_arr_size;
    Macro** macros;
} MacroList;

MacroList*  macrolist_new(int size);
void        macrolist_add(MacroList *self, Macro *macro);

#endif
