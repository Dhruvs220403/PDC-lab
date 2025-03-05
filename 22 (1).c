#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n = 800;
    int *array = (int *)malloc(n * sizeof(int));
    long long sum;
    double start_time, end_time;

    for (int i = 0; i < n; i++) {
        array[i] = i + 1;
    }

    int thread_counts[] = {2, 4, 6, 8 , 10 , 12 , 14 , 800};
    int num_tests = sizeof(thread_counts) / sizeof(thread_counts[0]);

    printf("Threads\tSum\t\tTime (seconds)\n");

    for (int t = 0; t < num_tests; t++) {
        int num_threads = thread_counts[t];
        sum = 0;

        omp_set_num_threads(num_threads);

        start_time = omp_get_wtime();

        #pragma omp parallel for reduction(+:sum)
        for (int i = 0; i < n; i++) {
            sum += array[i];
        }

        end_time = omp_get_wtime();

        printf("%d\t%lld\t%f\n", num_threads, sum, end_time - start_time);
    }

    free(array);
    return 0;
}

