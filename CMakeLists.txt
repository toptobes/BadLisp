cmake_minimum_required(VERSION 3.22)
project(my_other_lisp C)

set(CMAKE_C_STANDARD 99)

add_executable(my_other_lisp src/macros.c src/macros.h src/util/fileio.h src/util/fileio.c src/datastructures/list.h src/datastructures/list.c src/util/stack.h src/transpile.h src/transpile.c src/util/util.h src/util/util.c src/util/dynamic_string.c src/util/dynamic_string.h src/special_forms.c src/special_forms.h src/bad_lisp.c src/bad_lisp.h src/datastructures/vect.c src/datastructures/vect.h src/datastructures/list.h src/datastructures/list.h src/datastructures/list.h)
