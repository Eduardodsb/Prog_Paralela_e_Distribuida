#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"

int main (int argc, char *argv[]) {
int meu_ranque, num_procs, mensagem = 3, prox_mensagem;
MPI_Status estado;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if(meu_ranque == 0){
        MPI_Send(&mensagem, 1, MPI_INT,(meu_ranque+1)%num_procs,1, MPI_COMM_WORLD);
        printf("Iniciado - Processo %d enviou o valor %d\n", meu_ranque, mensagem);
    }

    while(mensagem != 0){
        
        MPI_Recv(&mensagem, 1, MPI_INT, (meu_ranque+num_procs-1)%num_procs, 1, MPI_COMM_WORLD, &estado);
        if(meu_ranque == 0){
            prox_mensagem = mensagem - 1;
            if(prox_mensagem == -1)
                printf("Processo %d recebeu %d \n", meu_ranque, mensagem);
            else
                printf("Processo %d recebeu %d e enviou o valor %d\n", meu_ranque, mensagem, prox_mensagem);
        }else{
            prox_mensagem = mensagem;
            printf("Processo %d recebeu %d e enviou o valor %d\n", meu_ranque, mensagem, prox_mensagem);
        }
        sleep(1); /*Utilizado para ajudar a manter a ordem dos prints*/
        MPI_Send(&prox_mensagem, 1, MPI_INT,(meu_ranque+1)%num_procs,1, MPI_COMM_WORLD);

    }

    printf("Processo %d Finalizado\n", meu_ranque);
    MPI_Finalize();           
    
    return 0;
}