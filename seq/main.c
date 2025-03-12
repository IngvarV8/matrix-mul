#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define STACK_LIMIT 50000 // Matrix size limit for stack allocation

void printMatrix(int **matrix, int size);
void populateMatrix(int **matrix, int size);
void multiplyMatrices_ijk(int **A, int **B, int **result, int size);
void multiplyMatrices_ikj(int **A, int **B, int **result, int size);
void strassenMultiplication(int **A, int **B, int **C, int size);
void multiplyZOrderStrassen(int **A, int **B, int **C, int size);
int **allocateMatrix(int size, int useHeap);
void freeMatrix(int **matrix, int size, int useHeap);

int main()
{
    srand(time(NULL));

    int sizes[] = {500, 1000, 2000}; // Matrix sizes

    for (int idx = 0; idx < 3; idx++)
    {
        int size = sizes[idx];
        int useHeap = (size > STACK_LIMIT); // Use heap if size exceeds stack limit

        printf("\nMultiplying matrices of size %dx%d...\n", size, size);

        // Allocate matrices
        int **matrix1 = allocateMatrix(size, 0);
        int **matrix2 = allocateMatrix(size, 0);
        int **matrix3 = allocateMatrix(size, 0);

        populateMatrix(matrix1, size);
        populateMatrix(matrix2, size);

        // IJK Multiplication
        multiplyMatrices_ijk(matrix1, matrix2, matrix3, size);

        // IKJ Multiplication
        multiplyMatrices_ikj(matrix1, matrix2, matrix3, size);

        int **matrix4 = allocateMatrix(size, 1);
        int **matrix5 = allocateMatrix(size, 1);
        int **matrix6 = allocateMatrix(size, 1);

        // IJK Multiplication
        multiplyMatrices_ijk(matrix4, matrix5, matrix6, size);

        // IKJ Multiplication
        multiplyMatrices_ikj(matrix4, matrix5, matrix6, size);

        /*
        // Strassen Algorithm
        if (size % 2 == 0) { // Strassen requires even-sized matrices
            strassenMultiplication(matrix1, matrix2, matrix3, size);
        }

        // Z-order + Strassen
        if (size % 2 == 0) {
            multiplyZOrderStrassen(matrix1, matrix2, matrix3, size);
        }
        */

        // Free matrices
        freeMatrix(matrix1, size, useHeap);
        freeMatrix(matrix2, size, useHeap);
        freeMatrix(matrix3, size, useHeap);
    }

    return 0;
}

// ===== Matrix Multiplication Algorithms =====

void multiplyMatrices_ijk(int **A, int **B, int **result, int size)
{
    clock_t start = clock();

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            result[i][j] = 0; // Fix: Initialize result before summing
            for (int k = 0; k < size; k++)
            {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    clock_t end = clock();
    printf("IJK Algorithm: %f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
}

void multiplyMatrices_ikj(int **A, int **B, int **result, int size)
{
    clock_t start = clock();

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            result[i][j] = 0; // Fix: Initialize result before summing
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            for (int j = 0; j < size; j++)
            {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    clock_t end = clock();
    printf("IKJ Algorithm: %f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
}

// ===== Matrix Allocation and Freeing =====

// Allocate a matrix dynamically (heap or stack)
int **allocateMatrix(int size, int useHeap)
{
    int **matrix;

    if (useHeap)
    {
        printf("Using heap...\n");
        matrix = (int **)malloc(size * sizeof(int *));
        for (int i = 0; i < size; i++)
        {
            matrix[i] = (int *)malloc(size * sizeof(int));
        }
    }
    else
    {
        printf("Using stack...\n");
        static int matrixStack[STACK_LIMIT][STACK_LIMIT]; // Static to avoid stack overflow
        matrix = (int **)malloc(size * sizeof(int *));    // Only for pointer indirection

        for (int i = 0; i < size; i++)
        {
            matrix[i] = matrixStack[i]; // Point to the static array
        }
    }
    return matrix;
}

// Free dynamically allocated matrix (only if heap was used)
void freeMatrix(int **matrix, int size, int useHeap)
{
    if (!useHeap)
        return; // Do not free stack-based allocation

    for (int i = 0; i < size; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

// ===== Utility Functions =====

void printMatrix(int **matrix, int size)
{
    printf("First 10x10 part of matrix:\n");
    for (int i = 0; i < 10 && i < size; i++)
    {
        for (int j = 0; j < 10 && j < size; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void populateMatrix(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = rand() % 100;
        }
    }
}
