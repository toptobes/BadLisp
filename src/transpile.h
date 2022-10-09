#ifndef PLAYGROUND_TRANSPILE_H
#define PLAYGROUND_TRANSPILE_H

#include "list.h"
#include "vect.h"
#include "macros.h"

char* preprocess(const char *code, int *error);
List* tokenize(const char *code, int *error);
char* expand(const List *list, int *error);

char* expand_function(const List *list, int *error);

#endif