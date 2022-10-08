#ifndef BAD_LISP_DYNAMIC_STRING_H
#define BAD_LISP_DYNAMIC_STRING_H

#include <stddef.h>

typedef struct {
    char*  str;
    size_t length;
    size_t backing_arr_size;
} DynamicString;

DynamicString* dstr_new(size_t);
DynamicString* dstr_new_copy(char*);
DynamicString* dstr_new_transfer(char**);

void  dstr_cpy(DynamicString *self, char* str);
void  dstr_cpy_ds(DynamicString *self, DynamicString* other);

void  dstr_cat(DynamicString *self, char* str);
void  dstr_cat_ds(DynamicString *self, DynamicString* other);

void  dstr_append(DynamicString *self, char c);
void  dstr_drop_last(DynamicString *self, int num);

void  dstr_replace(DynamicString *self, char old, char new);
void  dstr_clear(DynamicString *self);

char* dstr_destroy_wrapper(DynamicString **self_ptr);
void  dstr_destroy(DynamicString **self_ptr);

#endif