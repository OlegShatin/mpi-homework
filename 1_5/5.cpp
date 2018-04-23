#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
      int size, rank;

      MPI_Init(NULL, NULL);
      MPI_Comm_size(MPI_COMM_WORLD, &size);
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);

      int block;
      if (rank == 0) {
          //srand(time(NULL));
          const int MAX_NUMBERS = 12;
          int arr[MAX_NUMBERS];

          // запонение массива
          for (int i = 0; i < MAX_NUMBERS; i++) {
            arr[i] = rand() % 100;
            printf("%d ", arr[i]);
          }
          printf("\n");

          int block_size = MAX_NUMBERS/(size+1);
          int offset = block_size;
          printf("thread - %d\n", rank);

          // Вывод блока 0 потока
          for (int i = 0; i < block_size; i++) {
            printf("%d ", arr[i]);
          }
          printf("\n");

          for (int i = 1; i < size; i++) {

            // подмассив arr[]
            int tmp_arr[block_size];
            // заполняем значениями из arr[] со смещением на offset
            for (int k = 0; k < block_size; k++) {
              tmp_arr[k] = arr[k+offset];
            }
            MPI_Send(tmp_arr, block_size, MPI_INT, i, 0, MPI_COMM_WORLD);
            // printf("1-ый поток отправил данные c длиной блока: %d к %d потоку\n", block_size, i);
            offset+= block_size;
          }
      } else {
          MPI_Status status;
          MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
          MPI_Get_count(&status, MPI_INT, &block);
          int* buf = (int*)malloc(sizeof(int) * block);

          MPI_Recv(buf, block, MPI_INT, 0, 0,
                   MPI_COMM_WORLD, &status);
          printf("thread - %d recieved from 0\n", rank);
          for (int i = 0; i < block; i++) {
            printf("%d ", buf[i]);
          }
          printf("\n");
          free(buf);
      }
      MPI_Finalize();
}