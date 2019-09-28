/*
Cálculo do PI - Método do Trapézio (Integral) sequêncial.

Compilar: mpicc -o calcPI_integral_seq calcPI_integral_seq.c -lm
Executar: mpirun -np 1 ./calcPI_integral 100000000

OBS1: DEVE SER EXECUTADO COM APENAS UM PROCESSO, POIS É SEQUÊNCIAL.
OBS2: O argumento pode ser alterado, o mesmo representa o número de intervalos. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main (int argc, char **argv){
double t_inicial, t_final; 
int n, i, rc;
double  pi, h, x, sum = 0.0;
    
	if(argc != 2){
      	printf("Número incorreto de argumentos\n");
		exit(-1);
	}

	if (sscanf(argv[1], "%d", &n) != 1){  /*n é fornecido como segundo argumento */
   		/* 	Erro  */
   		exit(-1);

    }

 	t_inicial = MPI_Wtime();

	if (n != 0){ 
    	h=1.0/ (double) n;
		for (i=1; i <= n; i++){ 
			x = h * ((double)i - 0.5);
			sum += (4.0/(1.0 + x*x));
		}
        pi = h* sum;
	 	t_final = MPI_Wtime();
        printf ("valor aproximado de pi: %.16f \n", pi);
	    printf ("Tempo de execução %.5f\n", t_final - t_inicial);

	}
return 0;
}