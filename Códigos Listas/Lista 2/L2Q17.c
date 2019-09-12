#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main (int argc, char *argv[]) {
int  i, *valor, meu_ranque, raiz = 0, tam_vetor;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);

    

    if (meu_ranque == raiz) {
        tam_vetor = 3;
        valor = (int*) malloc(sizeof(int)*tam_vetor);
        valor[0] = 1;
        valor[1] = 2;
        valor[2] = 3;
        MPI_Bcast(valor, tam_vetor, MPI_INT, raiz, MPI_COMM_WORLD);
    }else{
        tam_vetor = 5;
        valor = (int*) malloc(sizeof(int)*tam_vetor);
        MPI_Bcast(valor, tam_vetor, MPI_INT, raiz, MPI_COMM_WORLD);
    }

    
    printf("O processo com ranque %d recebeu o valor: %d\n",meu_ranque, valor[0]);
    printf("O processo com ranque %d recebeu o valor: %d\n",meu_ranque, valor[1]);
    printf("O processo com ranque %d recebeu o valor: %d\n",meu_ranque, valor[2]);

    MPI_Finalize();
    return 0;
}
