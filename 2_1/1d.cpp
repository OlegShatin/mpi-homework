#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define BUF_LEN 256

int main(int argc, char **args){
    int my_rank;
    int size;

    MPI_Init(&argc, &args);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    if(my_rank == 0){
        int x[size];
        int y[size];
        for (int i = 1; i < size; i++){
            x[i] = i;
            y[i] = i * i;
            printf("x = %d  ", x[i]);
            printf("y = %d  ", y[i]);
        }
        printf("\n");
        for(int dest = 1; dest < size; dest++) {
            int arr[2];
            arr[0] = x[dest];
            arr[1] = y[dest];
            MPI_Send(arr, 2, MPI_INT,
                     dest, 0, MPI_COMM_WORLD);
            int res[2];
            MPI_Recv(res, 2, MPI_INT, dest,
                 0, MPI_COMM_WORLD, &status);
            x[dest] = res[0];
	    y[dest] = res[1];
	    printf("x = %d  ", x[dest]);
            printf("y = %d  ", y[dest]);
        }
	printf("\n");
    }
    else{
        int arr[2];
        MPI_Recv(arr, 2, MPI_INT, 0,
                 0, MPI_COMM_WORLD, &status);
        int x[2];
        x[0] = arr[1];
	x[1] = arr[0];
        MPI_Send(x, 2, MPI_INT,
                     0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize ();
    return 0;
}
