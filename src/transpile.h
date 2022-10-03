#ifndef PLAYGROUND_TRANSPILE_H
#define PLAYGROUND_TRANSPILE_H

#include "list.h"

char* preprocess(const char *code, int *error);
List* tokenize(char *code, int *error);
char* expand(List* list, int *error);

char* expand_default(List*, int *);

#endif