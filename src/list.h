#ifndef PLAYGROUND_LIST_H
#define PLAYGROUND_LIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct ListElement ListElement;

typedef struct List {
    char        *head;
    ListElement *rest;
    size_t      rest_c;
    size_t      backing_arr_size;
} List;

typedef struct Vector {
    char        *type;
    ListElement *rest;
    size_t      rest_c;
    size_t      backing_arr_size;
} Vect;

typedef enum {
    WORD,
    LIST,
    VECT,
} ListElement_t;

struct ListElement {
    ListElement_t type;
    union {
        List *as_list;
        char *as_word;
        Vect *as_vect;
    };
};

List* list_new();
void  list_print(const List *self, int depth);

void  list_add_word(List *self, const char* word, bool is_func_name);
void  list_add_list(List *self, List *list_to_nest);

#endif