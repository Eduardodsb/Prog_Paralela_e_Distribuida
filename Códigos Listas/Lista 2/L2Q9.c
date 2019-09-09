#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#define MAX_FLOAT 100

int main (int argc, char *argv[]) {
int meu_ranque, total_num, etiq = 20;
int raiz=0;
float vetor[MAX_FLOAT];
MPI_Status estado;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    
    if (meu_ranque != raiz) {
        srand(MPI_Wtime() * meu_ranque);
        total_num = (rand() / (float)RAND_MAX) * MAX_FLOAT;
        MPI_Send(vetor, total_num, MPI_FLOAT, raiz, etiq, MPI_COMM_WORLD);
        printf("Processo %d enviou %d números para 0\n", meu_ranque, total_num);
    } else if (meu_ranque == raiz) {
        MPI_Recv(vetor, MAX_FLOAT, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &estado);
        MPI_Get_count(&estado, MPI_INT, &total_num);
        printf("Processo %d recebeu %d números. Origem da mensagem = %d, etiqueta = %d\n", raiz, total_num, estado.MPI_SOURCE, estado.MPI_TAG);
    }
    
    MPI_Finalize();           
    
    return 0;
}