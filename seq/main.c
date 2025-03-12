#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printMatrix(int** matrix, int size);
void populateMatrix(int** matrix, int size);
void multiplyMatrices(int** A, int** B, int** result, int size);

int main() {
    srand(time(NULL));
    
    int sizes[] = {125, 250, 500, 1000, 2000};  // Array holding the matrix sizes to process
    
    for (int idx = 0; idx < 5; idx++) {
        int size = sizes[idx];

        // Dynamically allocate memory for the matrices based on the current size
        int** matrix1 = (int**)malloc(size * sizeof(int*));
        int** matrix2 = (int**)malloc(size * sizeof(int*));
        int** matrix3 = (int**)malloc(size * sizeof(int*));

        for (int i = 0; i < size; i++) {
            matrix1[i] = (int*)malloc(size * sizeof(int));
            matrix2[i] = (int*)malloc(size * sizeof(int));
            matrix3[i] = (int*)malloc(size * sizeof(int));
        }

        printf("\nMultiplying matrices of size %dx%d...\n", size, size);

        // Populate matrices with random values
        populateMatrix(matrix1, size);
        populateMatrix(matrix2, size);

        // Multiply matrices
        multiplyMatrices(matrix1, matrix2, matrix3, size);

        // Free the dynamically allocated memory
        for (int i = 0; i < size; i++) {
            free(matrix1[i]);
            free(matrix2[i]);
            free(matrix3[i]);
        }
        free(matrix1);
        free(matrix2);
        free(matrix3);
    }

    return 0;
}

void multiplyMatrices(int** A, int** B, int** result, int size) {
    // Initialize the result matrix with zeros
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = 0;
        }
    }

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    end_t = clock();

    total_t = (double)(end_t - start_t) * 1000.0 / CLOCKS_PER_SEC; // Time in ms
    printf("Total time taken for %dx%d matrix multiplication: %f ms\n", size, size, total_t);
}

void printMatrix(int** matrix, int size) {
    // Print only first 10x10 part of the matrix for debugging
    printf("Printing first 10x10 part of the matrix:\n");
    for (int i = 0; i < 10 && i < size; i++) { // Print only first 10 rows for debugging
        for (int j = 0; j < 10 && j < size; j++) { // Print only first 10 columns for debugging
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void populateMatrix(int** matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % 100;  // Random values between 0 and 99
        }
    }
}
