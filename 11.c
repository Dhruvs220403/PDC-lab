#include <stdio.h>
#include <omp.h>

double findA() {
    return 4;
}

double findB() {
    return 58;
}

int main() {
    double res = 0, sharedVar = 0;

    #pragma omp parallel
    {
        #pragma omp single
        {
            sharedVar = 45;
            printf("Shared variable initialized by thread -> %d\n", omp_get_thread_num());
        }

        #pragma omp sections reduction(+:res)
        {
            #pragma omp section
            {
                double resA = sharedVar * findA();
                printf("Section 1 computed by thread -> %d : %f\n", omp_get_thread_num(), resA);
                res += resA;
            }

            #pragma omp section
            {
                double resB = sharedVar * findB();
                printf("Section 2 computed by thread -> %d : %f\n", omp_get_thread_num(), resB);
                res += resB;
            }
        }
    }

    printf("Final result -> %f\n", res);
    return 0;
}
