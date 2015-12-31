#include <stdio.h>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#include "../timer.h"

int fib(const int n) {
    if (n < 2) {
        return n;
    }

    int x, y;
    x = cilk_spawn fib(n-1);
    y = cilk_spawn fib(n-2);
    cilk_sync;
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

    __cilkrts_set_param("nworkers", argv[2]);

    int result;
    timer t;

    //Run the benchmark and time it
    startTimer(&t);
    result = fib(n);
    stopTimer(&t);

    //Output the result as a comma-separated string
    printf("%d,%d,%d,%lf\n", n, atoi(argv[2]), result, t.ms);

    return 0;
}
