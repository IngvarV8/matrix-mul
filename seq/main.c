#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void printMatrixInt(int **matrix, int size);
void populateMatrixInt(int **matrix, int size);
void multiplyMatrices_ijk_int(int **A, int **B, int **result, int size);
void multiplyMatrices_ikj_int(int **A, int **B, int **result, int size);
void blockMultiply_int(int **A, int **B, int **C, int size, int blockSize);

void printMatrixFloat(float **matrix, int size);
void populateMatrixFloat(float **matrix, int size);
void multiplyMatrices_ijk_float(float **A, float **B, float **result, int size);
void multiplyMatrices_ikj_float(float **A, float **B, float **result, int size);
void blockMultiply_float(float **A, float **B, float **C, int size, int blockSize);

int **allocateMatrixInt(int size);
float **allocateMatrixFloat(int size);
void freeMatrixInt(int **matrix, int size);
void freeMatrixFloat(float **matrix, int size);

int main()
{
    srand(time(NULL));

    int sizes[] = {500, 1000, 2000}; // Matrix sizes

    for (int idx = 0; idx < 3; idx++)
    {
        int size = sizes[idx];

        printf("\nMultiplying matrices of size %dx%d...\n", size, size);

        // Allocate matrices for int
        int **matrix1_int = allocateMatrixInt(size);
        int **matrix2_int = allocateMatrixInt(size);
        int **matrix3_int = allocateMatrixInt(size);

        populateMatrixInt(matrix1_int, size);
        populateMatrixInt(matrix2_int, size);

        // IJK Multiplication (Int)
        multiplyMatrices_ijk_int(matrix1_int, matrix2_int, matrix3_int, size);
        //printMatrixInt(matrix3_int, size);

        // IKJ Multiplication (Int)
        multiplyMatrices_ikj_int(matrix1_int, matrix2_int, matrix3_int, size);
        //printMatrixInt(matrix3_int, size);

        blockMultiply_int(matrix1_int, matrix2_int, matrix3_int, size, 16);
        blockMultiply_int(matrix1_int, matrix2_int, matrix3_int, size, 32);
        blockMultiply_int(matrix1_int, matrix2_int, matrix3_int, size, 64);
        blockMultiply_int(matrix1_int, matrix2_int, matrix3_int, size, 128);
        //printMatrixInt(matrix3_int, size);

        // Free int matrices
        freeMatrixInt(matrix1_int, size);
        freeMatrixInt(matrix2_int, size);
        freeMatrixInt(matrix3_int, size);

        // Allocate matrices for float
        float **matrix1_float = allocateMatrixFloat(size);
        float **matrix2_float = allocateMatrixFloat(size);
        float **matrix3_float = allocateMatrixFloat(size);

        populateMatrixFloat(matrix1_float, size);
        populateMatrixFloat(matrix2_float, size);

        // IJK Multiplication (Float)
        multiplyMatrices_ijk_float(matrix1_float, matrix2_float, matrix3_float, size);
        //printMatrixFloat(matrix3_float, size);

        // IKJ Multiplication (Float)
        multiplyMatrices_ikj_float(matrix1_float, matrix2_float, matrix3_float, size);
        //printMatrixFloat(matrix3_float, size);

        blockMultiply_float(matrix1_float, matrix2_float, matrix3_float, size, 16);
        blockMultiply_float(matrix1_float, matrix2_float, matrix3_float, size, 32);
        blockMultiply_float(matrix1_float, matrix2_float, matrix3_float, size, 64);
        blockMultiply_float(matrix1_float, matrix2_float, matrix3_float, size, 128);
        //printMatrixFloat(matrix3_float, size);

        // Free float matrices
        freeMatrixFloat(matrix1_float, size);
        freeMatrixFloat(matrix2_float, size);
        freeMatrixFloat(matrix3_float, size);
    }

    return 0;
}

// ===== Matrix Multiplication Algorithms for Int =====

void multiplyMatrices_ijk_int(int **A, int **B, int **result, int size)
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

    printf("IJK Algorithm (Int): %f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
}

void multiplyMatrices_ikj_int(int **A, int **B, int **result, int size)
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

    printf("IKJ Algorithm (Int): %f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
}

void blockMultiply_int(int **A, int **B, int **C, int size, int blockSize)
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

    printf("Block Algorithm (Int, Block size = %d): %f ms\n", blockSize, (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
}

// ===== Matrix Multiplication Algorithms for Float =====

void multiplyMatrices_ijk_float(float **A, float **B, float **result, int size)
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

    printf("IJK Algorithm (Float): %f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
}

void multiplyMatrices_ikj_float(float **A, float **B, float **result, int size)
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

    printf("IKJ Algorithm (Float): %f ms\n", (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
}

void blockMultiply_float(float **A, float **B, float **C, int size, int blockSize)
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

    printf("Block Algorithm (Float, Block size = %d): %f ms\n", blockSize, (double)(end - start) * 1000.0 / CLOCKS_PER_SEC);
}

// ===== Matrix Allocation and Freeing for Int and Float =====

int **allocateMatrixInt(int size)
{
    int **matrix = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
    {
        matrix[i] = (int *)malloc(size * sizeof(int));
    }
    return matrix;
}

float **allocateMatrixFloat(int size)
{
    float **matrix = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; i++)
    {
        matrix[i] = (float *)malloc(size * sizeof(float));
    }
    return matrix;
}

void freeMatrixInt(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void freeMatrixFloat(float **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

// ===== Utility Functions for Int and Float =====

void printMatrixInt(int **matrix, int size)
{
    printf("First 10x10 part of int matrix:\n");
    for (int i = 0; i < 10 && i < size; i++)
    {
        for (int j = 0; j < 10 && j < size; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void printMatrixFloat(float **matrix, int size)
{
    printf("First 10x10 part of float matrix:\n");
    for (int i = 0; i < 10 && i < size; i++)
    {
        for (int j = 0; j < 10 && j < size; j++)
        {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void populateMatrixInt(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = rand() % 100;
        }
    }
}

void populateMatrixFloat(float **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = (float)(rand() % 100);
        }
    }
}
