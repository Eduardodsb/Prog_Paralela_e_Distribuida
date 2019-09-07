#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
/*#define TAM_VET 10*/

int main (int argc, char *argv[]) {
int i, meu_ranque, num_procs, raiz = 0;
int *vet_envia, *vet_recebe/*[TAM_VET]*/;
int TAM_VET;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	/* O processo raiz aloca o espaço de memória e inicia o vetor */
	if (meu_ranque == raiz) {
        scanf("%d", &TAM_VET);
	    vet_envia = (int*) malloc (num_procs*TAM_VET*sizeof(int));

        printf("Vetor de envio = ");
	    for (i = 0; i < (TAM_VET*num_procs); i++){ 
            vet_envia[i] = i;
            printf("%d ", vet_envia[i]);
        }
        puts("");
    }

    MPI_Bcast(&TAM_VET, 1, MPI_INT, raiz, MPI_COMM_WORLD);
    vet_recebe = (int*) malloc (TAM_VET*sizeof(int));

    /* O vetor é distribuído em partes iguais entre os processos, incluindo o processo raiz */
	MPI_Scatter(vet_envia, TAM_VET, MPI_INT, vet_recebe, TAM_VET, MPI_INT, raiz, MPI_COMM_WORLD);/*TAM_VET não é o tamanho do vet_envia, é o tamanho do pedaço dado a cada processo*/
    /*OBS: É sempre particionado na ordem do ranque*/
    
	/* Cada processo imprime a parte que recebeu */ 
	printf("Processo =  %d, recebeu", meu_ranque);
	for (i = 0; i < TAM_VET; i ++) 
		printf(" %d", vet_recebe[i]);
	printf("\n");

	/* Termina a execução */
	MPI_Finalize();
	return 0;
}
