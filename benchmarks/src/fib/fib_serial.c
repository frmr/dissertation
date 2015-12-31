#include <stdio.h>
#include <stdlib.h>

#include "../timer.h"

int fib(const int n) {
    if (n < 2) {
        return n;
    }

    int x, y;

    x = fib(n-1);
    y = fib(n-2);

    return x + y;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: fib n\n");
        return 1;
    }

    if (atoi(argv[1]) < 1) {
        printf("Invalid target number: %d\n", atoi(argv[1]));
        return 1;
    }

    const int n = atoi(argv[1]);

    int result;
    timer t;

    startTimer(&t);
    result = fib(n);
    stopTimer(&t);

    printf("%d,%d,%d,%lf\n", n, 1, result, t.ms);

    return 0;
}
