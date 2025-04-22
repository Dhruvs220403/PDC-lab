#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, numprocs;
    int tag = 100, msg_size = 64;
    char *buf;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (numprocs != 3) {
        if (rank == 0) {
            printf("This program requires exactly 3 processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    printf("MPI Process %d started...\n", rank);
    fflush(stdout);

    while (msg_size < 1000000) {
        buf = (char*)malloc(msg_size * sizeof(char));

        if (rank == 0) {
            printf("Process %d sending message of size %d to Process 1\n", rank, msg_size);
            fflush(stdout);
            MPI_Send(buf, msg_size, MPI_BYTE, 1, tag, MPI_COMM_WORLD);
            MPI_Recv(buf, msg_size, MPI_BYTE, 2, tag, MPI_COMM_WORLD, &status);
            printf("Process %d received message of size %d from Process 2\n", rank, msg_size);
        } 
        else if (rank == 1) {
            MPI_Recv(buf, msg_size, MPI_BYTE, 0, tag, MPI_COMM_WORLD, &status);
            printf("Process %d received message of size %d from Process 0\n", rank, msg_size);
            MPI_Send(buf, msg_size, MPI_BYTE, 2, tag, MPI_COMM_WORLD);
            printf("Process %d sent message of size %d to Process 2\n", rank, msg_size);
        } 
        else if (rank == 2) {
            MPI_Recv(buf, msg_size, MPI_BYTE, 1, tag, MPI_COMM_WORLD, &status);
            printf("Process %d received message of size %d from Process 1\n", rank, msg_size);
            MPI_Send(buf, msg_size, MPI_BYTE, 0, tag, MPI_COMM_WORLD);
            printf("Process %d sent message of size %d to Process 0\n", rank, msg_size);
        }

        free(buf);
        msg_size *= 2; // Double the message size
        MPI_Barrier(MPI_COMM_WORLD); // Ensure synchronization before next iteration
    }

    MPI_Finalize();
    return 0;
}