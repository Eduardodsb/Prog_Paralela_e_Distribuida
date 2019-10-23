#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define MAXSIZE 65536

int main(int argc, char *argv[]){
    int my_rank, p, successor, predecessor, a ;
    int vetor[MAXSIZE], i, tam_buffer;
    void *buffer;


    MPI_Status estado;
    MPI_Request pedido_envia, pedido_recebe;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    for(i = 0; i < MAXSIZE; i++)
        vetor[i] = my_rank;    
    a = my_rank;

    MPI_Pack_size(MAXSIZE * 2 + 2 , MPI_INT, MPI_COMM_WORLD, &tam_buffer);
    tam_buffer = tam_buffer + 2*MPI_BSEND_OVERHEAD;
    buffer = (void *) malloc(tam_buffer);

    MPI_Buffer_attach(buffer, tam_buffer);
    
    successor = (my_rank + 1) % p;
    predecessor = (my_rank - 1 + p) % p;
    MPI_Bsend(&a, 1, MPI_FLOAT, successor, 1, MPI_COMM_WORLD);
    MPI_Bsend(vetor, MAXSIZE, MPI_FLOAT, successor, 1, MPI_COMM_WORLD);
    
    MPI_Recv(&a, 1, MPI_FLOAT, predecessor, 1, MPI_COMM_WORLD, &estado);
    MPI_Recv(vetor, MAXSIZE, MPI_FLOAT, predecessor, 1,MPI_COMM_WORLD, &estado);

    printf("Eu processo %d recebi a = %d e vetor[0] = %d\n", my_rank, a, vetor[0]);

    MPI_Finalize();
    return 0;
}