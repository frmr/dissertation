#include <stdio.h>
#include <stdlib.h>

#include "../timer.h"

int copyArray(int* arrIn, int* arrOut, const int n) {
    for (int i = 0; i < n; ++i) {
        arrOut[i] = arrIn[i];
    }
}

int sumArray(int* arr, const int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += arr[i];
    }
    return sum;
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
        //Copy only actually necessary for parallel versions, 
        //but do it for serial for speed comparison
        int* newQueens = malloc(n * sizeof(int));
        copyArray(oldQueens, newQueens, n);
        newQueens[row] = col;

        // try to fill next row
        int* solutions = malloc(n * sizeof(int));
        for (int i = 0; i < n; ++i) {
            solutions[i] = setQueen(newQueens, n, row + 1, i);
        }
        free(newQueens);

        //serial
        const int sum = sumArray(solutions, n);
        free(solutions);
        return sum;
    }
}

int solve(int* queens, const int n) {
    int* solutions = malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
        // try all positions in first row
        // create separate array for each recursion
        solutions[i] = setQueen(queens, n, 0, i);
    }

    //serial
    const int sum = sumArray(solutions, n);
    free(solutions);
    return sum;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: nqueens n\n");
        return 1;
    }

    if (atoi(argv[1]) < 1) {
        printf("Invalid number of queens: %d\n", atoi(argv[1]));
        return 1;
    }

    const int n = atoi(argv[1]);

    int* queens = malloc(n * sizeof(int));

    int solutions;
    timer t;

    startTimer(&t);
    solutions = solve(queens, n);
    stopTimer(&t);

    free(queens);

    printf("%d,%d,%d,%lf\n", n, 1, solutions, t.ms);

    return 0;
}