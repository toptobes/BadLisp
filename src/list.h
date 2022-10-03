#ifndef PLAYGROUND_LIST_H
#define PLAYGROUND_LIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct ListElement ListElement;

typedef struct List {
    char *func_name;
    ListElement *args;
    size_t argc;
} List;

typedef enum {
    WORD,
    LIST,
} ListElement_t;

struct ListElement {
    ListElement_t type;
    union {
        List *as_list;
        char *as_word;
    };
};

List* list_new();
void list_print(List*, int);

void list_add_string(List*, const char*, bool);
void list_add_list(List*, List*);

#endif