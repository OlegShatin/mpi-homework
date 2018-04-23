#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define BUF_LEN 256

int main(int argc, char **args){
    int my_rank;
    int p;
    int message[10];
    for (int i = 0; i < 10; i++){
	message[i] = i;
    }

    MPI_Init(&argc, &args);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Status status;

    if(my_rank == 1){
	printf("threads = %d ", p);
        for(int dest = 0; dest < p; dest++) {
	    if(dest == 1){
		continue;
	    }
	    printf("destination = %d\n", dest);
            MPI_Send(message, 10, MPI_INT,
                     dest, 0, MPI_COMM_WORLD);
        }
    }
    else{
	printf("my id is %d\n", my_rank);
	MPI_Probe(1, 0, MPI_COMM_WORLD, &status);
	int count;
	MPI_Get_count(&status, MPI_INT, &count);
	MPI_Recv(message, count, MPI_INT, 1,
                 0, MPI_COMM_WORLD, &status);
	for(int i = 0; i < 10; i++){
            printf ("%d ", message[i]);
        }
        printf("\n");
    }

    MPI_Finalize ();
    return 0;
}
