#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 100

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
    fp = fopen("array.dat", "wb");
    fwrite(array, sizeof(int), ARRAY_SIZE, fp);
    fclose(fp);

    return 0;
}