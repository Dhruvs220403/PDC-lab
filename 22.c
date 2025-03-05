#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000000

double sharedVar = 5.0;
double privateVar;
#pragma omp threadprivate(privateVar)

double computeA() {
    return 3.14;
}

double computeB() {
    return 2.71;
}

int main(void) {
    int i;
    int *vec1 = malloc(N * sizeof(int));
    int *vec2 = malloc(N * sizeof(int));
    
    double serialDot = 0.0;
    double parallelDot = 0.0;
    double startTime, endTime, serialTime, parallelTime;
    double compResultA = 0.0, compResultB = 0.0;
    
    srand((unsigned) time(NULL));
    for (i = 0; i < N; i++) {
        vec1[i] = (rand() % 100) + 1;
        vec2[i] = (rand() % 100) + 1;
    }
    
    startTime = omp_get_wtime();
    for (i = 0; i < N; i++) {
        serialDot += (double)vec1[i] * vec2[i];
    }
    endTime = omp_get_wtime();
    serialTime = endTime - startTime;
    
    startTime = omp_get_wtime();
    #pragma omp parallel for reduction(+:parallelDot)
    for (i = 0; i < N; i++) {
        parallelDot += (double)vec1[i] * vec2[i];
    }
    endTime = omp_get_wtime();
    parallelTime = endTime - startTime;
    
    #pragma omp parallel
    {
        privateVar = (double) omp_get_thread_num();
        #pragma omp single
        {
            printf("Starting independent parallel section computations...\n");
        }
        #pragma omp sections
        {
            #pragma omp section
            {
                compResultA = computeA() * sharedVar * privateVar;
            }
            #pragma omp section
            {
                compResultB = computeB() * sharedVar * privateVar;
            }
        }
    }
    
    printf("Serial Dot Product:   %f\n", serialDot);
    printf("Parallel Dot Product: %f\n", parallelDot);
    
    printf("Result from computeA section: %f\n", compResultA);
    printf("Result from computeB section: %f\n", compResultB);
    
    printf("Serial Dot Product Time:   %f seconds\n", serialTime);
    printf("Parallel Dot Product Time: %f seconds\n", parallelTime);
    
    free(vec1);
    free(vec2);
    
    return 0;
}	
