#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define NUM_TESTS 10

int main(int argc, char *argv[]) {
    int rank, num_procs;
    double start_time, end_time, elapsed_time;
    double bandwidth;
    int i, size;
    MPI_Status status;
    double *buffer;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (num_procs != 2) {
        if (rank == 0)
            printf("Error: This test requires exactly 2 MPI processes.\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0)
        printf("Message size (Bytes)\tBandwidth (MB/sec)\n");

    for (size = 1; size <= 100000000; size *= 10) {
        // Allocate buffer for messages
        buffer = (double *)malloc(size * sizeof(double));
        if (!buffer) {
            printf("Error: Could not allocate buffer of size %d bytes\n", size * (int)sizeof(double));
            break;
        }

        // Synchronize before timing
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == 0) {
            // Sender process
            start_time = MPI_Wtime();
            for (i = 0; i < NUM_TESTS; i++) {
                MPI_Send(buffer, size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(buffer, size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &status);
            }
            end_time = MPI_Wtime();
        } else if (rank == 1) {
            // Receiver process
            for (i = 0; i < NUM_TESTS; i++) {
                MPI_Recv(buffer, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Send(buffer, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }
        }

        // Calculate and print bandwidth
        if (rank == 0) {
            elapsed_time = (end_time - start_time) / NUM_TESTS;
            bandwidth = (2.0 * size * sizeof(double)) / (elapsed_time * 1e6); // in MB/sec
            printf("%d\t\t\t%.2f\n", size * (int)sizeof(double), bandwidth);
        }

        free(buffer);
    }

    MPI_Finalize();
    return 0;
}