#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 1000000

int main() {
    int i;
    int array[ARRAY_SIZE];

    // Initialize array with random values
    srand(time(NULL));
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand();
    }

    // Save array to file
    FILE* fp;
    fp = fopen("array.dat", "w, ccs=UTF-8");
    for (i = 0; i < ARRAY_SIZE; i++) {
        fprintf(fp, "%d\n", array[i]);
    }
    fclose(fp);

    return 0;
}