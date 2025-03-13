#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void printMatrix(int **matrix, int size);
void populateMatrix(int **matrix, int size);
void multiplyMatrices_ijk(int **A, int **B, int **result, int size);
void multiplyMatrices_ikj(int **A, int **B, int **result, int size);
int **allocateMatrix(int size);
void freeMatrix(int **matrix, int size);
void blockMultiply(int **A, int **B, int **C, int size, int blockSize);

int main()
{
    srand(time(NULL));

    int sizes[] = {500, 1000, 2000}; // Matrix sizes

    for (int idx = 0; idx < 3; idx++)
    {
        int size = sizes[idx];

        printf("\nMultiplying matrices of size %dx%d...\n", size, size);

        // Allocate matrices
        int **matrix1 = allocateMatrix(size);
        int **matrix2 = allocateMatrix(size);
        int **matrix3 = allocateMatrix(size);

        populateMatrix(matrix1, size);
        populateMatrix(matrix2, size);

        // IJK Multiplication
        multiplyMatrices_ijk(matrix1, matrix2, matrix3, size);
        printMatrix(matrix3, size);

        // IKJ Multiplication
        multiplyMatrices_ikj(matrix1, matrix2, matrix3, size);
        printMatrix(matrix3, size);

        blockMultiply(matrix1, matrix2, matrix3, size, 16);
        blockMultiply(matrix1, matrix2, matrix3, size, 32);
        blockMultiply(matrix1, matrix2, matrix3, size, 64);
        blockMultiply(matrix1, matrix2, matrix3, size, 128);
        printMatrix(matrix3, size);

        // Free matrices
        freeMatrix(matrix1, size);
        freeMatrix(matrix2, size);
        freeMatrix(matrix3, size);
    }

    return 0;
}

// ===== Matrix Multiplication Algorithms =====

void multiplyMatrices_ijk(int **A, int **B, int **result, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            result[i][j] = 0; 
        }
    }
    
    // CLOCK START
    clock_t start = clock();
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    clock_t end = clock();
    // CLOCK END

    printf("IJK Algorithm: %f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
}

void multiplyMatrices_ikj(int **A, int **B, int **result, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            result[i][j] = 0; 
        }
    }

    // CLOCK START
    clock_t start = clock();
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
    // CLOCK END

    printf("IKJ Algorithm: %f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
}

void blockMultiply(int **A, int **B, int **C, int size, int blockSize)
{
    // CLOCK START
    clock_t start = clock();
    for (int i = 0; i < size; i += blockSize)
    {
        for (int j = 0; j < size; j += blockSize)
        {
            for (int k = 0; k < size; k += blockSize)
            {
                // Multiply blocks A(i, k) * B(k, j) and add to C(i, j)
                for (int ii = i; ii < i + blockSize && ii < size; ii++)
                {
                    for (int jj = j; jj < j + blockSize && jj < size; jj++)
                    {
                        for (int kk = k; kk < k + blockSize && kk < size; kk++)
                        {
                            C[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }
    clock_t end = clock();
    // CLOCK END

    printf("Block Algorithm (Block size = %d): %f ms\n", blockSize, (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
}


// ===== Matrix Allocation and Freeing =====

// Allocate a matrix dynamically
int **allocateMatrix(int size)
{
    int **matrix;

    matrix = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
    {
        matrix[i] = (int *)malloc(size * sizeof(int));
    }

    return matrix;
}

// Free dynamically allocated matrix
void freeMatrix(int **matrix, int size)
{
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
