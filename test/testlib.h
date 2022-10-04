#ifndef PLAYGROUND_TESTLIB_H
#define PLAYGROUND_TESTLIB_H

#include <stdio.h>

#define $EXPECT_RESULT(result) printf(result ";")
#define $ACTUAL_RESULT(fmt, ...) printf(fmt, __VA_ARGS__)

#endif
