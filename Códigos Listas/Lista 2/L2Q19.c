/*Deve ser executado com 3 processos*/
/*mpirun -np 3 ./L2Q19*/

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#define TAM_VET 4

int main (int argc, char *argv[]) {
int i, meu_ranque, num_procs, raiz = 0;
int *vet_envia, vet_recebe[TAM_VET];
int *desloc, *cont_envia, salto=3;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
        MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

        if (meu_ranque == raiz) {
            vet_envia = (int*) malloc (10*sizeof(int));
            for (i = 0; i < 10; i++)
                vet_envia[i] = i;
        }
        /* Prepara os parametros para a chamada da rotina */
        desloc = (int *)malloc(num_procs*sizeof(int));
        cont_envia = (int *)malloc(num_procs*sizeof(int));
        cont_envia[0] = 3;
        cont_envia[1] = 3;
        cont_envia[2] = 4;
        for (i=0; i<num_procs; ++i) {
                desloc[i] = i*salto;;
        }

       /* Pulando salto elementos entre cada parte enviada a cada processo */
        MPI_Scatterv(vet_envia, cont_envia, desloc, MPI_INT, vet_recebe, TAM_VET, MPI_INT, raiz, MPI_COMM_WORLD);
        
        /* Cada processo imprime a parte que recebeu */
        if(meu_ranque == 0 || meu_ranque == 1 ){
            printf("Processo =  %d, recebeu", meu_ranque);
            for (i = 0; i < 3; i ++)
                printf(" %d", vet_recebe[i]);
            printf("\n");
        }else{
            printf("Processo =  %d, recebeu", meu_ranque);
            for (i = 0; i < 4; i ++)
                printf(" %d", vet_recebe[i]);
            printf("\n");
        }
        
        MPI_Finalize();
        return 0;
}

