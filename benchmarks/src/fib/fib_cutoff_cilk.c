#include <stdio.h>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#include "../timer.h"

int fib(const int n, const int cutoff) {
    if (n < 2) {
        return n;
    }

    int x, y;
    if (n < cutoff) {
        x = fib(n-1, cutoff);
        y = fib(n-2, cutoff);
    }
    else {
        x = cilk_spawn fib(n-1, cutoff);
        y = cilk_spawn fib(n-2, cutoff);
        cilk_sync;
    }
    return x + y;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: fib n workers cutoff\n");
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
    
    if (atoi(argv[3]) < 1) {
        printf("Invalid cutoff: %d\n", atoi(argv[3]));
    }

    const int n = atoi(argv[1]);
    __cilkrts_set_param("nworkers", argv[2]);
    const int cutoff = atoi(argv[3]);

    int result;
    timer t;

    startTimer(&t);
    result = fib(n, cutoff);
    stopTimer(&t);

    printf("%d,%d,%d,%d,%lf\n", n, atoi(argv[2]), atoi(argv[3]), result, t.ms);

    return 0;
}
