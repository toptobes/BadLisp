#ifndef PLAYGROUND_TRANSPILE_H
#define PLAYGROUND_TRANSPILE_H

#include "list.h"
#include "macros.h"

char* preprocess(const char*, int*);
List* tokenize(char*, int*);
List* reg_macros(List*, MacroList*, int*);
char* expand(List*, MacroList*, int*);

char* expand_function(List *list, int *error);

#endif