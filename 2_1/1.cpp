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
	    int x[1];
	    MPI_Recv(x, 1, MPI_INT, dest,
                 0, MPI_COMM_WORLD, &status);
	    printf("z[%d] = ", dest - 1);
	    printf("%d\n", x[0]);
        }
    }
    else{
	int arr[2];
        MPI_Recv(arr, 2, MPI_INT, 0,
                 0, MPI_COMM_WORLD, &status);
	int x[1];
	x[0] = arr[0] + arr[1] * 2;
        MPI_Send(x, 1, MPI_INT,
                     0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize ();
    return 0;
}
