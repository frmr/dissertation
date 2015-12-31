#include <stdio.h>
#include <stdlib.h>

#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

#include "../timer.h"

//Copy contents of arrIn to arrOut
int copyArray(int* arrIn, int* arrOut, const int n) {
    cilk_for (int i = 0; i < n; ++i) {
        arrOut[i] = arrIn[i];
    }
}

//Sum the contents arr and return the result
int sumArray(int* arr, const int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += arr[i];
    }
    return sum;
}

//Try setting a queen at the board position defined by row and col
//If the board is valid, try setting queens in every column of the next row
//If this is the last row, board state is a valid solution
int setQueen(int* oldQueens, const int n, const int row, const int col) {
    //Check if placing a queen at (row, col) creates a valid board state
    for (int i = 0; i < row; ++i) {
        if (oldQueens[i] == col) {
            return 0;
        }
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

        //Try to fill the next row and count the solutions found
        int* solutions = malloc(n * sizeof(int));
        cilk_for (int i = 0; i < n; ++i) {
            solutions[i] = setQueen(newQueens, n, row + 1, i);
        }
        free(newQueens);

        //Serial operation
        const int sum = sumArray(solutions, n);
        free(solutions);
        return sum;
    }
}

//Try all positions in first row
//Create separate array for each recursion
int solve(int* queens, const int n) {
    int* solutions = malloc(n * sizeof(int));
    cilk_for (int i = 0; i < n; ++i) {
        solutions[i] = setQueen(queens, n, 0, i);
    }

    //Serial operation
    const int sum = sumArray(solutions, n);
    free(solutions);
    return sum;
}

int main(int argc, char* argv[]) {
    //Check that arguments are valid
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

    //Run the benchmark and time it
    startTimer(&t);
    solutions = solve(queens, n);
    stopTimer(&t);

    free(queens);

    //Output the result as a comma-separated string
    printf("%d,%d,%d,%lf\n", n, atoi(argv[2]), solutions, t.ms);

    return 0;
}
