#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#define TAM_VET 15

int main (int argc, char *argv[]) {
int i, meu_ranque, num_procs, raiz = 0;
int *vet_envia, vet_recebe[TAM_VET];
int *desloc, *cont_envia, salto=TAM_VET + 5;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
        MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
        /* Aloca o espaço de memória e inicia o vetor */
        if (meu_ranque == raiz) {
            vet_envia = (int*) malloc (num_procs*salto*sizeof(int));
            for (i = 0; i < (salto*num_procs); i++)
                vet_envia[i] = i;
        }
        
        /* Prepara os parametros para a chamada da rotina */
        desloc = (int *)malloc(num_procs*sizeof(int));
        cont_envia = (int *)malloc(num_procs*sizeof(int));
        for (i=0; i<num_procs; ++i) {
                desloc[i] = i*salto;
                cont_envia[i] = TAM_VET;
        }
       /* Distribui o vetor em partes iguais a TAM_VET entre os processos */
       /* Pulando salto elementos entre cada parte enviada a cada processo */
        MPI_Scatterv(vet_envia, cont_envia, desloc, MPI_INT, vet_recebe, TAM_VET, MPI_INT, raiz, MPI_COMM_WORLD);
        /* Cada processo imprime a parte que recebeu */
        printf("Processo =  %d, recebeu", meu_ranque);
        for (i = 0; i < TAM_VET; i ++)
                printf(" %d", vet_recebe[i]);
                        printf("\n");
        /* Termina a execução */
        MPI_Finalize();
        return 0;
}
