#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define SIZE 100

double privateVar;
#pragma omp threadprivate(privateVar)

int main(void) {
    int i, j;
    double mat[SIZE][SIZE], vec[SIZE], result[SIZE];
    double sum_result = 0.0, max_val = 0.0;
    
    srand(time(NULL));
    for (i = 0; i < SIZE; i++) {
        vec[i] = (double)rand() / RAND_MAX;
        for (j = 0; j < SIZE; j++) {
            mat[i][j] = (double)rand() / RAND_MAX;
        }
    }
    
    for (i = 0; i < SIZE; i++) {
        result[i] = 0.0;
    }
    
    #pragma omp parallel
    {
        privateVar = omp_get_thread_num();
        #pragma omp single
        {
            printf("Starting matrix-vector multiplication in parallel...\n");
        }
        #pragma omp for reduction(+:result[:SIZE])
        for (i = 0; i < SIZE; i++) {
            for (j = 0; j < SIZE; j++) {
                result[i] += mat[i][j] * vec[j];
            }
        }
    }
    
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            double local_sum = 0.0;
            for (i = 0; i < SIZE; i++) {
                local_sum += result[i];
            }
            sum_result = local_sum;
        }
        #pragma omp section
        {
            double local_max = result[0];
            for (i = 1; i < SIZE; i++) {
                if (result[i] > local_max)
                    local_max = result[i];
            }
            max_val = local_max;
        }
    }
    
    printf("Sum of all elements in result: %f\n", sum_result);
    printf("Maximum value in result: %f\n", max_val);
    
    return 0;
}

