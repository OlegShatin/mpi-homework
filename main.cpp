#include <iostream>
#include <stdio.h>
#include <cmath>

#include "mpi.h"
#include "integral.h"
//Все параметры для простоты задаются константами
static double a=0; // левый и правый концы интеграла
static double b=1;


int n = 1000000000; // число точек разбиения
static double total; // результат интегрирования

/* Функция, работающая в процессе с номером my_rank,
при общем числе процессов р. */
void process_function (int my_rank, int p){
    /* длина отрезка интегрирования для текущего процесса*/
 double len = (b - a) / p;

/* число точек разбиения для текущего процесса */
 int local_n = n / p;
    /* левый конец интервала для текущего процесса */
    double local_a = a + my_rank * len;
    /* правый конец интервала для текущего процесса */
    double local_b = local_a + len;
    /* значение интеграла в текущем процессе */
    double integral;
    /* Вычислить интеграл в каждом из процессов */
    integral = integrate (local_a, local_b, local_n);
   // Сложить все ответы и передать процессу О
    MPI_Reduce (&integral, &total, 1, MPI_DOUBLE, MPI_SUM,    0, MPI_COMM_WORLD);
}


using namespace std;

int main (int argc, char **argv)
{


int my_rank;
/* ранг текущего процесса */
int p;
/* общее число процессов */
/* Начать работу с MPI */
MPI_Init (&argc, &argv);
double t;

//Синхронизация всех процессов *I
MPI_Barrier(MPI_COMM_WORLD);

//I* Время начала */
t = MPI_Wtime();

/*
Получить номер текущего процесса в группе всех
процессов */
MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
/* Получить общее количество запущенных процессов */
MPI_Comm_size (MPI_COMM_WORLD, &p);
/* Вычислить интеграл в каждом из процессов */
process_function (my_rank, p);
/* Напечатать ответ в процессе О */
if (my_rank == 0)
 cout << "Integral from " << a << " to " << b << " = " << total  << endl;

//Синхронизация всех процессов *I
MPI_Barrier(MPI_COMM_WORLD);

//I* Время начала */
t = MPI_Wtime()-t;

 cout << "Calculation time " << t  << endl;
/* Заканчиваем работу с MPI */
MPI_Finalize ();

//    cout << "MPI_Finalize, process number " << my_rank << endl;

    return 0;

}


//  mpirun -n 6 ./Integral_MPI
