/*
Cálculo do PI - Método do Trapézio (Integral) paralelo.

Compilar: mpicc -o calcPI_integral_para calcPI_integral_para.c -lm
Executar: mpirun -np 8 ./calcPI_integral_para 100000000

OBS1: Pode ser alterado o número de processos.
OBS2: O argumento pode ser alterado, o mesmo representa o número de intervalos.  
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main (int argc, char **argv){ 
double t_inicial, t_final;
int n, i, meu_ranque, num_procs, raiz = 0;
double  pi, h, x, sum, minha_sum = 0.0;
    

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	
    if(meu_ranque == raiz){
        if(argc != 2){
            printf("Número incorreto de argumentos\n");
            exit(-1);
        }

        if (sscanf(argv[1], "%d", &n) != 1){  /*n é fornecido como segundo argumento*/
            exit(-1);
        }
    }

    t_inicial = MPI_Wtime();

    MPI_Bcast(&n, 1, MPI_INT, raiz, MPI_COMM_WORLD);

	if (n != 0){ 
    	h=1.0/ (double) n;
		for (i= meu_ranque + 1; i <= n; i+=num_procs){ 
			x = h * ((double)i - 0.5);
			minha_sum += (4.0/(1.0 + x*x));
		}

        MPI_Reduce(&minha_sum,&sum, 1, MPI_DOUBLE, MPI_SUM, raiz, MPI_COMM_WORLD); 

        t_final = MPI_Wtime();
        if(meu_ranque == raiz){
            pi = h* sum;
            printf ("valor aproximado de pi: %.16f \n", pi);
            printf ("Tempo de execução %.5f\n", t_final - t_inicial);
        }
	}

    
    MPI_Finalize();
    return 0;
}