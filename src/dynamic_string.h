#ifndef MY_OTHER_LISP_DYNAMIC_STRING_H
#define MY_OTHER_LISP_DYNAMIC_STRING_H

#include <stddef.h>

typedef struct {
    char *str;
    size_t length;
    size_t total_size;
} DynamicString;

DynamicString* dstr_new(size_t);
DynamicString* dstr_new_copy(char*);
DynamicString* dstr_new_transfer(char**);

void dstr_cpy(DynamicString*, char*);
void dstr_cpy_ds(DynamicString*, DynamicString*);

void dstr_cat(DynamicString*, char*);
void dstr_cat_ds(DynamicString*, DynamicString*);

void dstr_append(DynamicString*, char);
void dstr_drop_last(DynamicString*, int);

void dstr_replace(DynamicString*, char, char);
void dstr_clear(DynamicString*);

char* dstr_destroy_wrapper(DynamicString**);
void dstr_destroy(DynamicString**);

#endif