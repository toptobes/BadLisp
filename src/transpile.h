#ifndef PLAYGROUND_TRANSPILE_H
#define PLAYGROUND_TRANSPILE_H

#include "datastructures/list.h"
#include "datastructures/vect.h"
#include "macros.h"

char* preprocess(const char *code, int *error);
List* tokenize(const char *code, int *error);
char* expand_list(const List *list, int *error);
char* expand_vect(const Vect *vector, int *error);
char* expand_non_words_default(const List *list, int from_index, int *error);
char* expand_function(const List *list, int *error);

#endif