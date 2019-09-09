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
        do{
            mensagem = (char*) malloc(sizeof(char)*200);
            sprintf(mensagem, "Oi processo %d sou o processo 0", num_procs -1);
            MPI_Send(mensagem, strlen(mensagem)+1, MPI_CHAR,num_procs -1,1, MPI_COMM_WORLD);
        } while (--num_procs);
    } else{
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
        MPI_Get_count(&estado, MPI_CHAR, &tam_mensagem);
        mensagem = (char*) malloc(sizeof(char)*tam_mensagem);

/*Versão 1*/  MPI_Recv(mensagem, tam_mensagem, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &estado);
/*Versão 2*/ // MPI_Recv(mensagem, tam_mensagem, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
/*Versão 3*/ // MPI_Recv(mensagem, tam_mensagem, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        printf("Processo %d - Mensagem recebida: %s \n", meu_ranque, mensagem);
    }
    
    MPI_Finalize();           
    
    return 0;
}