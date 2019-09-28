/*
Ordena Vetor - Código Paralelo.

Compilar: mpicc -o ordena_para ordena_para.c -lm
Executar: mpirun -np 8 ./ordena_para 16000000

OBS1: Pode ser alterado o número de processos.
OBS2: O argumento pode ser alterado, o mesmo representa o tamanho do vetor.  
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h>

int main(int argc, char **argv) {
int i, meu_ranque, num_procs, raiz = 0;
int *vetor, tamVetor;
double t_inicial, t_final; 
srand( time(NULL) );

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if(meu_ranque == raiz){
        if(argc != 2){
            printf("Número incorreto de argumentos\n");
            exit(-1);
        }
        sscanf(argv[1], "%d", &tamVetor);
        if((vetor = (int*) malloc(sizeof(int)*tamVetor)) == NULL){
            printf("Erro ao alocar o vetor na memória\n");
            exit(-1);
        }

        for(i = 0; i < tamVetor; i++){
            vetor[i] = rand();
        }
    }


    if(meu_ranque == raiz)
        free(vetor);
    MPI_Finalize();
    return 0;
}