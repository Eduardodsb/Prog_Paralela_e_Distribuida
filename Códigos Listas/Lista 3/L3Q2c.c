#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define MAXSIZE 65536

int main(int argc, char *argv[]){
    int my_rank, p, successor, predecessor, a ;
    int vetor[MAXSIZE], i;
    MPI_Status estado;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    for(i = 0; i < MAXSIZE; i++)
        vetor[i] = my_rank;    
    a = my_rank;
    
    successor = (my_rank + 1) % p;
    predecessor = (my_rank - 1 + p) % p;

    if(my_rank%2 == 0){
        MPI_Ssend(&a, 1, MPI_FLOAT, successor, 1, MPI_COMM_WORLD);
        MPI_Ssend(vetor, MAXSIZE, MPI_FLOAT, successor, 1, MPI_COMM_WORLD);
        MPI_Recv(&a, 1, MPI_FLOAT, predecessor, 1, MPI_COMM_WORLD, &estado);
        MPI_Recv(vetor, MAXSIZE, MPI_FLOAT, predecessor, 1,MPI_COMM_WORLD, &estado);
    }else{
        MPI_Recv(&a, 1, MPI_FLOAT, predecessor, 1, MPI_COMM_WORLD, &estado);
        MPI_Recv(vetor, MAXSIZE, MPI_FLOAT, predecessor, 1,MPI_COMM_WORLD, &estado);
        MPI_Ssend(&a, 1, MPI_FLOAT, successor, 1, MPI_COMM_WORLD);
        MPI_Ssend(vetor, MAXSIZE, MPI_FLOAT, successor, 1, MPI_COMM_WORLD);
    }
    printf("Eu processo %d recebi a = %d e vetor[0] = %d\n", my_rank, a, vetor[0]);

    MPI_Finalize();
    return 0;
}