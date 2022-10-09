cmake_minimum_required(VERSION 3.22)
project(my_other_lisp C)

set(CMAKE_C_STANDARD 99)

add_executable(my_other_lisp src/macros.c src/macros.h src/fileio.h src/fileio.c src/list.h src/list.c src/stack.h src/transpile.h src/transpile.c src/util.h src/util.c src/dynamic_string.c src/dynamic_string.h src/special_forms.c src/special_forms.h src/bad_lisp.c src/bad_lisp.h src/vect.c src/vect.h)
