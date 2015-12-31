#include <stdio.h>
#include <stdlib.h>

#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#include "../timer.h"

int copyArray(int* arrIn, int* arrOut, const int n) {
    cilk_for (int i = 0; i < n; ++i) {
        arrOut[i] = arrIn[i];
    }
}

int setQueen(int* oldQueens, const int n, const int row, const int col) {
    for (int i = 0; i < row; ++i) {
        // vertical attacks
        if (oldQueens[i] == col) {
            return 0;
        }
        // diagonal attacks
        if (abs(oldQueens[i] - col) == (row - i) ) {
            return 0;
        }
    }

    if (row == n - 1) {
        return 1;
    }
    else {
        //Copy array and set queen
        int* newQueens = malloc(n * sizeof(int));
        copyArray(oldQueens, newQueens, n);
        newQueens[row] = col;

        // try to fill next row
        int solutions = 0;
        cilk_for (int i = 0; i < n; ++i) {
            solutions += setQueen(newQueens, n, row + 1, i);
        }

        free(newQueens);
        return solutions;
    }
}

int solve(int* queens, const int n) {
    int solutions = 0;
    cilk_for (int i = 0; i < n; ++i) {
        // try all positions in first row
        solutions += setQueen(queens, n, 0, i);
    }
    return solutions;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: nqueens n workers\n");
        return 1;
    }

    if (atoi(argv[1]) < 1) {
        printf("Invalid number of queens: %d\n", atoi(argv[1]));
        return 1;
    }

    if (atoi(argv[2]) < 1) {
        printf("Invalid number of workers: %d\n", atoi(argv[2]));
        return 1;
    }

    const int n = atoi(argv[1]);

    __cilkrts_set_param("nworkers", argv[2]);

    int* const queens = malloc(n * sizeof(int));

    int solutions;
    timer t;

    startTimer(&t);
    solutions = solve(queens, n);
    stopTimer(&t);

    free(queens);

    printf("%d,%d,%d,%lf\n", n, atoi(argv[2]), solutions, t.ms);

    return 0;
}