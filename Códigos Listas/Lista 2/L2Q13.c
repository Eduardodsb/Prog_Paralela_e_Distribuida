#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

int main (int argc, char *argv[]) {
int meu_ranque, num_procs, tam_mensagem ;
char *mensagem;
MPI_Status estado;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (meu_ranque == 0) {
        mensagem = (char*) malloc(sizeof(char)*200);
        sprintf(mensagem, "Oi processo sou o processo %d", meu_ranque);
        MPI_Send(mensagem, strlen(mensagem)+1, MPI_CHAR,1,1, MPI_COMM_WORLD);
    }else if(meu_ranque == 1){
        MPI_Probe(0, 1, MPI_COMM_WORLD, &estado);
        MPI_Get_count(&estado, MPI_CHAR, &tam_mensagem);
        mensagem = (char*) malloc(sizeof(char)*tam_mensagem);

        MPI_Recv(mensagem, tam_mensagem, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &estado);
        
        printf("Processo %d - Mensagem recebida: %s - Tamanho da msg: %d \n", meu_ranque, mensagem,tam_mensagem);
    }
    
    MPI_Finalize();           
    
    return 0;
}