#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <omp.h>

#include "../timer.h"

//Merge two contiguous sections of the given array
void merge(int* const arr, const size_t start, const size_t split, const size_t end) {
    const size_t n = (end - start) + 1;
    int* const tmpArr = malloc(n * sizeof(int));

    size_t ai = start;
    size_t bi = split + 1;
    size_t ci = 0;

    while (ci < n) {
        if (ai <= split && bi <= end) {
            tmpArr[ci++] = (arr[ai] < arr[bi]) ? arr[ai++] : arr[bi++];
        }
        else if (ai <= split) {
            tmpArr[ci++] = arr[ai++];
        }
        else if (bi <= end) {
            tmpArr[ci++] = arr[bi++];
        }
    }

    //Copy tmpArr into arr
    for (ci = 0; ci < n; ++ci) {
        arr[start+ci] = tmpArr[ci];
    }

    free(tmpArr);
}

//Merge sort the given array
void msort(int* const arr, const size_t start, const size_t end) {
    if (start == end) {
        return;
    }

    const size_t split = start + (end - start) / 2;

    #pragma omp task shared(arr)
    msort(arr, start, split);

    #pragma omp task shared(arr)
    msort(arr, split + 1, end);

    #pragma omp taskwait

    merge(arr, start, split, end);
}

//Check that the given array is sorted
int sorted(const int* const arr, const size_t n) {
    for (int i = 0; i < n - 1; ++i) {
        if (arr[i] > arr[i+1]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[]) {
    //Check that arguments are valid
    if (argc != 3) {
        printf("Usage: msort n workers\n");
        return 1;
    }

    if (atoi(argv[1]) < 1) {
        printf("Invalid array size: %d\n", atoi(argv[1]));
        return 1;
    }

    if (atoi(argv[2]) < 1) {
        printf("Invalid number of workers: %d\n", atoi(argv[2]));
        return 1;
    }

    const int n = atoi(argv[1]);

    omp_set_num_threads(atoi(argv[2]));

    int* arr = malloc(n * sizeof(int));

    //Fill array with random values
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        arr[i] = rand();
    }

    timer t;

    //Run the benchmark and time it
    startTimer(&t);
    #pragma omp parallel shared(arr)
    {
        #pragma omp single nowait
        {
            msort(arr, 0, n - 1);
        }
    }
    stopTimer(&t);

    int success = sorted(arr, n);

    //Output the result as a comma-separated string
    printf("%d,%d,%d,%lf\n", n, atoi(argv[2]), success, t.ms);

    free(arr);
    return 0;
}
