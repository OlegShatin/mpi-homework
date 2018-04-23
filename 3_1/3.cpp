#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char **args){
    int rank;
    int size;

    MPI_Init(&argc, &args);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    float array[size * 2][2];

    float local_min[4], global_min[4];
    if (rank == 0) {
        for (int i = 0; i < size * 2; ++i) {
            array[i][0] = (float)rand()/RAND_MAX;
            array[i][1] = i;
            printf("%f\n", array[i][0]);
        }
    }
    MPI_Scatter(array, 2, MPI_FLOAT_INT, &local_min, 2, MPI_FLOAT_INT, 0, MPI_COMM_WORLD);
    float x[2];
    if (local_min[0] < local_min[2]){
	x[0] = local_min[0];
	x[1] = local_min[1];
    }
    else{
	x[0] = local_min[2];
	x[1] = local_min[3];
    }
    printf("This is thread number %d and my value is %f with index %d\n", rank, x[0], (int)x[1]);   
    MPI_Reduce(&x, &global_min, 1, MPI_FLOAT_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Min value is %f\n", global_min[0]);
        printf("Global index is %d\n", (int)global_min[1]);
    }
    MPI_Finalize();
    return 0;
}
