#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include "dynamic_string.h"

DynamicString* dstr_new(size_t max_str_length)
{
    DynamicString *ds = malloc(sizeof(*ds));

    ds->str = malloc(max_str_length + 1);
    ds->str[0] = '\0';
    ds->length = 0;
    ds->backing_arr_size = max_str_length + 1;

    return ds;
}

DynamicString* dstr_new_copy(char *initial_str)
{
    DynamicString *ds = malloc(sizeof(*ds));

    ds->str = strdup(initial_str);
    ds->length = strlen(ds->str);
    ds->backing_arr_size = ds->length + 1;

    return ds;
}

DynamicString* dstr_new_transfer(char **str_ptr)
{
    DynamicString *ds = malloc(sizeof(*ds));

    ds->str = malloc(strlen(*str_ptr));
    ds->str[0] = '\0';
    ds->length = strlen(*str_ptr);
    ds->backing_arr_size = strlen(*str_ptr) + 1;

    strcat(ds->str, *str_ptr);
    free(*str_ptr);
    *str_ptr = NULL;

    return ds;
}

void dstr_cpy(DynamicString *self, char *str)
{
    self->length = strlen(str);

    bool max_size_has_changed = self->length >= self->backing_arr_size;

    while (self->length >= self->backing_arr_size)
    {
        self->backing_arr_size *= 2;
    }

    if (max_size_has_changed)
    {
        self->str = realloc(self->str, self->backing_arr_size);
        self->str[self->length + 1] = '\0';
    }

    strcpy(self->str, str);
}

void dstr_cpy_ds(DynamicString *self, DynamicString *other)
{
    dstr_cpy(self, other->str);
}

void dstr_cat(DynamicString *self, char *str)
{
    self->length += strlen(str);

    bool max_size_has_changed = self->length >= self->backing_arr_size;

    while (self->length >= self->backing_arr_size)
    {
        self->backing_arr_size *= 2;
    }

    if (max_size_has_changed)
    {
        self->str = realloc(self->str, self->backing_arr_size);
        self->str[self->length + 1] = '\0';
    }

    strcat(self->str, str);
}

void dstr_cat_ds(DynamicString *self, DynamicString *other)
{
    dstr_cat(self, other->str);
}

void dstr_append(DynamicString *self, char c)
{
    self->length++;

    if (self->length >= self->backing_arr_size)
    {
        self->str = realloc(self->str, (self->backing_arr_size *= 2));
    }

    self->str[self->length - 1] = c;
    self->str[self->length] = '\0';
}

void dstr_drop_last(DynamicString *self, int num)
{
    self->length -= num;
    self->str[self->length] = '\0';
}

void dstr_replace(DynamicString *self, char old, char new)
{
    for (int i = 0; i < self->length; i++)
    {
        if (self->str[i] == old)
        {
            self->str[i] = new;
        }
    }
}

void dstr_clear(DynamicString *self)
{
    self->length = 0;
    self->str[0] = '\0';
}


char* dstr_destroy_wrapper(DynamicString **self_ptr)
{
    char *str = (*self_ptr)->str;
    free(*self_ptr);
    *self_ptr = NULL;
    return str;
}


void dstr_destroy(DynamicString **self_ptr)
{
    char *str = (*self_ptr)->str;
    free(*self_ptr);
    *self_ptr = NULL;
    free(str);
}