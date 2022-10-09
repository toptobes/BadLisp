#ifndef MY_OTHER_LISP_VECT_H
#define MY_OTHER_LISP_VECT_H

#include "list.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct Vector {
    char        *type;
    ListElement *elems;
    size_t      elems_c;
    size_t      backing_arr_size;
} Vect;

Vect* vect_new();
void  vect_print(const Vect *self, int depth);

void  vect_set_type(Vect *self, const char* type);
void  vect_add_word(Vect *self, const char* word);
void  vect_add_list(Vect *self, List *list);
void  vect_add_vect(Vect *self, Vect *nested_vect);

#endif