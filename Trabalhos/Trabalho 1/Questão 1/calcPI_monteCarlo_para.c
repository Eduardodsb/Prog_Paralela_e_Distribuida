/*
Cálculo do PI - Método de Monte Carlo paralelo.

Compilar: mpicc -o calcPI_monteCarlo_para calcPI_monteCarlo_para.c -lm
Executar: mpirun -np 8 ./calcPI_monteCarlo_para 100000000

OBS1: Pode ser alterado o número de processos.
OBS2: O argumento pode ser alterado, o mesmo representa o o número de experimentos.  
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define SEED time(NULL)

int main(int argc, char **argv) {
double t_inicial, t_final; 
int meu_ranque, num_procs, raiz = 0;
unsigned long int i, count, n, count_total;
double x,y,z,pi;
srand( SEED );

    count = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	
    if(meu_ranque == raiz){
        if(argc != 2){
            printf("Número incorreto de argumentos\n");
            exit(-1);
        }

        if (sscanf(argv[1], "%ld", &n) != 1){  /*n é fornecido como segundo argumento*/
            exit(-1);
        }
        
    }

    t_inicial = MPI_Wtime();
    MPI_Bcast(&n, 1, MPI_LONG, raiz, MPI_COMM_WORLD);

    for(i = meu_ranque; i < n; i+=num_procs) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        z = x * x + y * y;
        if( z <= 1 ) count++;
    }

    MPI_Reduce(&count,&count_total, 1, MPI_LONG, MPI_SUM, raiz, MPI_COMM_WORLD); 
    t_final = MPI_Wtime();

    if(meu_ranque == raiz){
        pi = (double) count_total / n * 4;
        printf("Aproximação de PI é = %.16f\n", pi);
        printf ("Tempo de execução %.5f\n", t_final - t_inicial);
    }

    MPI_Finalize();

 return(0);
}