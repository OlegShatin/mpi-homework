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
        float x[size - 1][size - 1];
        float y[size - 1][size - 1];
        for (int i = 0; i < size - 1; i++){
	    for (int j = 0; j < size; j++){
        	x[i][j] = (i + 1) / (double)(j + 1);
        	y[i][j] = (j + 1) / (double)(i + 1);
		//y[i][j] = 2;
	    }
        }
	for (int i = 0; i < size - 1; i++){
            for (int j = 0; j < size - 1; j++){
                printf("%f ", x[i][j]);
            }
            printf("\n");
	}
        printf("\n");
        for (int i = 0; i < size - 1; i++){
            for (int j = 0; j < size - 1; j++){ 
                printf("%f ", y[i][j]);
            }
            printf("\n");
	}
        printf("\n");

	for (int i = 0; i < size - 1; i++){
	    float sub[(size - 1) * 2];
	    int z = 0;
	    for (int j = 0; j < size - 1; j ++){
		sub[z] = x[i][j];
		z += 1;
	    }
	    for (int j = 0; j < size - 1; j ++){
                sub[z] = y[i][j];
                z += 1;
            }

//	    for (int j = 0; j < (size - 1) * 2; j++){
//		printf("%f ", sub[j]);
//	    }
	    printf("\n");

	    MPI_Send(sub, (size - 1) * 2, MPI_INT,
                     i + 1, 0, MPI_COMM_WORLD);

	    float res[size - 1];
	    MPI_Recv(res, size - 1, MPI_INT, i + 1,
                       0, MPI_COMM_WORLD, &status);
	    for (int j = 0; j < size - 1; j++){
		printf("%f ", res[j]);
	    }
	    printf("\n");
	}
    }
    else{
	MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        int count;
        MPI_Get_count(&status, MPI_INT, &count);
        float arr[count];
        MPI_Recv(arr, count, MPI_INT, 0,
                 0, MPI_COMM_WORLD, &status);
//	for (int j = 0; j < count; j++){
//                printf("%f ", arr[j]);
//        }
	printf("\n");
        float q[size - 1];
        for (int i = 0; i < size - 1; i++){
	    q[i] = arr[i] * arr[size - 1 + i];
	}
        MPI_Send(q, count / 2, MPI_INT,
                     0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize ();
    return 0;
}
