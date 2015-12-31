#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    //copy tmpArr in arr
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
    msort(arr, start, split);
    msort(arr, split + 1, end);
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
    if (argc != 2) {
        printf("Usage: msort n\n");
        return 1;
    }

    if (atoi(argv[1]) < 1) {
        printf("Invalid array size: %d\n", atoi(argv[1]));
        return 1;
    }
    const int n = atoi(argv[1]);

    int* const arr = malloc(n * sizeof(int));

    //fill array with random values
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        arr[i] = rand();
    }

    timer t;

    startTimer(&t);
    msort(arr, 0, n - 1);
    stopTimer(&t);

    int success = sorted(arr, n);

    printf("%d,%d,%d,%lf\n", n, 1, success, t.ms);

    free(arr);
    return 0;
}