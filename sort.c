#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define ARRAY_SIZE 1000000

int compare(const void* a, const void* b) {
    const int* x = (const int*) a;
    const int* y = (const int*) b;
    return (*x < *y) ? -1 : (*x > *y);
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* array, int low, int high, int (*compare)(const void*, const void*)) {
    int pivot = array[high];
    int i = low - 1;
    int j;

    #pragma omp parallel for shared(array, pivot, low, high, i) private(j)
    for (j = low; j <= high - 1; j++) {
        if (compare(&array[j], &pivot) <= 0) {
            i++;
            swap(&array[i], &array[j]);
        }
    }

    swap(&array[i + 1], &array[high]);

    return i + 1;
}

void quicksort(int* array, int low, int high, int (*compare)(const void*, const void*)) {
    if (low < high) {
        int pi = partition(array, low, high, compare);

        quicksort(array, low, pi - 1, compare);
        quicksort(array, pi + 1, high, compare);
    }
}

void my_qsort(void* base, size_t num, size_t size, int (*compare)(const void*, const void*)) {
    int* array = (int*) base;
    int i;

    quicksort(array, 0, num - 1, compare);
}

int main() {
    int i;
    int array[ARRAY_SIZE];

    // Read array from file
    FILE* fp;
    fp = fopen("array.dat", "r, ccs=UTF-8");
    for (i = 0; i < ARRAY_SIZE; i++) {
        fscanf(fp, "%d", &array[i]);
    }
    fclose(fp);

    // Sort the array using my_qsort()
    double start_time = omp_get_wtime();
    my_qsort(array, ARRAY_SIZE, sizeof(int), compare);
    double end_time = omp_get_wtime();

    // Print sorted array
    for (i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    printf("Execution time: %f seconds\n", end_time - start_time);

    return 0;
}
