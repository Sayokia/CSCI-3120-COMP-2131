#include <stdio.h>
#include <time.h>
#include <stdlib.h>





int main() {
    int i;
    int N = 100000;
    int M = 100;
    int arr[N];
    int sum;
    clock_t start, finish;
    double  duration;
    srand((unsigned)time(NULL));
    for(int i = 0; i < N; i++)
    {
        arr[i] = rand();
    }

    start = clock();

    int length = 100000;

    for (i=0;i<length;i++) {
        sum += arr[i];

    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("%d\n",sum);
    printf( "%f seconds\n", duration );



    return 0;

}

