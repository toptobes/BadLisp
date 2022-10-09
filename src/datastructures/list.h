#ifndef PLAYGROUND_LIST_H
#define PLAYGROUND_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include "vect.h"

struct Vect;

typedef struct List {
    char                *head;
    struct ListElement  *rest;
    size_t              rest_c;
    size_t              backing_arr_size;
} List;

typedef enum {
    WORD,
    LIST,
    VECT,
} ListElement_t;

typedef union ListElementCast {
    List        *list;
    char        *word;
    struct Vect *vect;
} ListElementCast;

typedef struct ListElement {
    ListElement_t type;
    ListElementCast as;
} ListElement;

List* list_new();
void  list_print(const List *self, int depth);

void  list_add_word(List *self, const char* word, bool is_func_name);
void  list_add_list(List *self, List *nested_list);
void  list_add_vect(List *self, struct Vect *vector);

#endif