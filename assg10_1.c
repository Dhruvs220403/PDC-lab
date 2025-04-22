#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    int rank, size;
    int i, n, test;
    int NUMBER_OF_TESTS = 100;
    double t1, t2, total_time;
    double* msg;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
        printf("Message Size (doubles)\tAvg. Latency per round-trip [ms]\n");

    for (n = 1; n <= 1000; n *= 10) {
        msg = (double*)malloc(n * sizeof(double));
        for (i = 0; i < n; i++) msg[i] = 1.0;

        total_time = 0.0;

        for (test = 0; test < NUMBER_OF_TESTS; test++) {
            MPI_Barrier(MPI_COMM_WORLD);

            if (rank == 0) {
                t1 = MPI_Wtime();
                MPI_Send(msg, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(msg, n, MPI_DOUBLE, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                t2 = MPI_Wtime();
                total_time += (t2 - t1);
            } else {
                MPI_Recv(msg, n, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int next = (rank + 1) % size;
                MPI_Send(msg, n, MPI_DOUBLE, next, 0, MPI_COMM_WORLD);
            }
        }

        if (rank == 0) {
            double avg_latency_ms = (total_time / NUMBER_OF_TESTS) * 1000.0;
            printf("%d\t\t\t%.3f\n", n, avg_latency_ms);
        }

        free(msg);
    }

    MPI_Finalize();
    return 0;
}
