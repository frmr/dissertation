#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "../timer.h"

int fib(const int n) {
    if (n < 2) {
        return n;
    }

    int x, y;

    #pragma omp task shared(x)
    x = fib(n-1);

    #pragma omp task shared(y)
    y = fib(n-2);

    #pragma omp taskwait

    return x + y;
}

int main(int argc, char* argv[]) {
    //Check that arguments are valid
    if (argc != 3) {
        printf("Usage: fib n workers\n");
        return 1;
    }

    if (atoi(argv[1]) < 1) {
        printf("Invalid target number: %d\n", atoi(argv[1]));
        return 1;
    }

    if (atoi(argv[2]) < 1) {
        printf("Invalid number of workers: %d\n", atoi(argv[2]));
        return 1;
    }

    const int n = atoi(argv[1]);

    omp_set_num_threads(atoi(argv[2]));

    int result;
    timer t;

    //Run the benchmark and time it
    startTimer(&t);
    #pragma omp parallel shared(result)
    {
        #pragma omp single nowait
        {
            result = fib(n);
        }
    }
    stopTimer(&t);

    //Output the result as a comma-separated string
    printf("%d,%d,%d,%lf\n", n, atoi(argv[2]), result, t.ms);

    return 0;
}
