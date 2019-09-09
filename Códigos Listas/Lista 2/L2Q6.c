#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[]) {

    int meu_ranque, num_procs, origem, destino, etiq=0;
    char mensagem[200];
    MPI_Status	info;

    MPI_Init(&argc, &argv);

    if (MPI_Initialized){ 
         printf("MPI iniciado! \n");
    }else{
        MPI_Init(&argc, &argv);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
	
    if (meu_ranque != 0)  {
        sprintf(mensagem, "Processo %d está vivo!", meu_ranque);
        destino = 0;
        MPI_Send(mensagem,strlen(mensagem)+1,MPI_CHAR,destino,etiq, MPI_COMM_WORLD);
   } else {
        for (origem=1; origem < num_procs; origem++) {
            MPI_Recv(mensagem, 200, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &info);
            printf("%s\n",mensagem); 
        }
   } 

    MPI_Finalize();

    if (MPI_Finalized){
        printf("MPI finalizado! \n");
    }else{
        MPI_Finalize();
    }

    return 0;
}
