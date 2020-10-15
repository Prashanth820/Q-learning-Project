#include <stdio.h>
#include <time.h>
 
int main (void)
{
    double diff = 0.0;
    time_t start;
    time_t stop;
    char buff[128];
 
    printf("Quick! Input!!!\n");
    time(&start);
    if (fgets(buff, sizeof buff, stdin) != NULL) {
        time(&stop);
        diff = difftime(stop, start);
        printf("&#37;s\n", buff);
        printf("It took you %g seconds to type that\n", diff);
    }
    return 0;
}