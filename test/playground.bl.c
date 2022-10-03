#include <time.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char** argv){srand(time(NULL));{int n1 = rand();int n2 = rand();char buf[2] = {};printf("What is %d * %d? ",n1,n2);if((NULL==fgets(buf,2000,stdin))){{puts("\ninvalid input");return(1);};}else{{int answer=atoi(buf);;if((answer==(n1*n2))){puts("\nCorrect");}else{printf("\nNo; the answer was %d\n",(n1*n2));};};};};}