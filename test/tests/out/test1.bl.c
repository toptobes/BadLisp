#include "testlib.h"
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#define NUM_1 1
#define NUM_2 2
#define MAX(a,b) (a>b?a:b)
char* str_from_fmt(char* fmt,...);int main(void){$EXPECT_RESULT("72");if((NUM_1==MAX(NUM_1,NUM_2))){puts("This won't happen");}else{{char* s = str_from_fmt("%s %d","Hello",123);int len = strlen(s);int lx8 = (len<<3);$ACTUAL_RESULT("%d",lx8);};};return(0);}char* str_from_fmt(char* fmt,...){{va_list argv = NULL;va_start(argv,fmt);{int length = vsnprintf(NULL,0,fmt,argv);char* buff = malloc((length+1));vsprintf(buff,fmt,argv);va_end(argv);return(buff);};};}