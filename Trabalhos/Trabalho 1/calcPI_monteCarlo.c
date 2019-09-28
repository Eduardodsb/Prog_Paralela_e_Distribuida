/*Autor: Adrian Statescu mergesortv@gmail.com http://adrianstatescu.com
Descrição:  Programa em C Program para computar o valor de PI usando o método de Monte Carlo.
Licença MIT 
_______________________________________________________________________________________________________
Cálculo do PI - Método de Monte Carlo sequêncial.

Compilar: mpicc -o calcPI_monteCarlo calcPI_monteCarlo.c  -lm
Executar: mpirun -np 1 ./calcPI_monteCarlo 100000000

OBS1: DEVE SER EXECUTADO COM APENAS UM PROCESSO, POIS É SEQUÊNCIAL.
OBS2: O argumento pode ser alterado, o mesmo representa o número de intervalos.  
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define SEED time(NULL)

int main() {
double t_inicial, t_final; 
int i, count, n=1000000000;
double x,y,z,pi;
srand( SEED );

    printf("n = %d \n", n);
    count = 0;

    t_inicial = MPI_Wtime();
    for(i = 0; i < n; ++i) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        z = x * x + y * y;
        if( z <= 1 ) count++;
    }

    pi = (double) count / n * 4;
    t_final = MPI_Wtime();
    printf("Aproximação de PI é = %.16f\n", pi);
    printf ("Tempo de execução %.5f\n", t_final - t_inicial);

 return(0);
}
