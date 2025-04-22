#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    int rank, size;
    int n;
    int* full_data = NULL;
    int* sub_data = NULL;
    int* gathered_data = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            printf("This program must be run with exactly 4 processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        n = 2;
        full_data = (int*)malloc(4 * n * sizeof(int));
        for (int i = 0; i < 4 * n; i++)
            full_data[i] = i + 1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    sub_data = (int*)malloc(n * sizeof(int));
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Scatter(full_data, n, MPI_INT, sub_data, n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n; i++)
        sub_data[i] *= 2;

    printf("Process %d received: ", rank);
    for (int i = 0; i < n; i++)
        printf("%d ", sub_data[i]);
    printf("\n");

    if (rank == 0)
        gathered_data = (int*)malloc(4 * n * sizeof(int));

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(sub_data, n, MPI_INT, gathered_data, n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Final gathered array in process 0: ");
        for (int i = 0; i < 4 * n; i++)
            printf("%d ", gathered_data[i]);
        printf("\n");
    }

    free(sub_data);
    if (rank == 0) {
        free(full_data);
        free(gathered_data);
    }

    MPI_Finalize();
    return 0;
}