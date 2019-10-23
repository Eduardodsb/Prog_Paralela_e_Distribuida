#include <stdio.h>
#include "mpi.h"

#define MAXSIZE 65536

int main(int argc, char *argv[]){
    int my_rank, p, successor, predecessor, a ;
    int vetor[MAXSIZE], i;
    MPI_Status estado;
    MPI_Request pedido_envia, pedido_recebe;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    for(i = 0; i < MAXSIZE; i++)
        vetor[i] = my_rank;    
    a = my_rank;

    successor = (my_rank + 1) % p;
    predecessor = (my_rank - 1 + p) % p;
    MPI_Isend(&a, 1, MPI_FLOAT, successor, 1, MPI_COMM_WORLD, &pedido_envia);
    MPI_Irecv(&a, 1, MPI_FLOAT, predecessor, 1, MPI_COMM_WORLD, &pedido_recebe);

    MPI_Wait(&pedido_envia,&estado);
    MPI_Wait(&pedido_recebe,&estado);

    MPI_Isend(vetor, MAXSIZE, MPI_FLOAT, successor, 1, MPI_COMM_WORLD, &pedido_envia);
    MPI_Irecv(vetor, MAXSIZE, MPI_FLOAT, predecessor, 1,MPI_COMM_WORLD, &pedido_recebe);

    MPI_Wait(&pedido_envia,&estado);
    MPI_Wait(&pedido_recebe,&estado);

    printf("Eu processo %d recebi a = %d e vetor[0] = %d\n", my_rank, a, vetor[0]);

    MPI_Finalize();
    return 0;
}