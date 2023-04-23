#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define ARRAY_SIZE 10000000

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

int is_valid_index(int index, int low, int high) {
    return (index >= low && index <= high);
}

int is_valid_pivot(int pivot, int low, int high) {
    return is_valid_index(pivot, low, high - 1);
}

int partition(int* array, int low, int high, int (*compare)(const void*, const void*)) {
    int pivot = array[high];
    int i = low - 1;
    int j;

    for (j = low; j <= high - 1; j++) {
        if (compare(&array[j], &pivot) <= 0) {
            i++;
            if (i != j && is_valid_index(i, low, high - 1) && is_valid_index(j, low, high - 1)) {
                swap(&array[i], &array[j]);
            }
        }
    }

    int pivot_index = i + 1;
    if (is_valid_pivot(pivot_index, low, high)) {
        swap(&array[pivot_index], &array[high]);
        return pivot_index;
    } else {
        return low - 1;
    }
}


void quicksort(int* array, int low, int high, int (*compare)(const void*, const void*)) {
    if (low < high) {
        int pi = partition(array, low, high, compare);

        #pragma omp task shared(array,compare)
        quicksort(array, low, pi - 1, compare);
        #pragma omp task shared(array,compare)
        quicksort(array, pi + 1, high, compare);
    }
}

void my_qsort(void* base, size_t num, size_t size, int (*compare)(const void*, const void*)) {
    int* array = (int*) base;

    #pragma omp parallel shared(array,compare)
    {
        #pragma omp single
        quicksort(array, 0, num - 1, compare);
    }
    #pragma omp taskwait
}

int main(int argc, char** argv) {
    int i;
    int* array = (int*) malloc(ARRAY_SIZE * sizeof(int));

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

    // Write sorted array to file
    fp = fopen(argv[1], "w, ccs=UTF-8");
    fprintf(fp, "Execution time: %f seconds\n", end_time - start_time);
    for (i = 0; i < ARRAY_SIZE; i++) {
        fprintf(fp, "%d\n", array[i]);
    }
    fclose(fp);

    free(array);

    return 0;
}