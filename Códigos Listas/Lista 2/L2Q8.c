#include <stdio.h>
#include "mpi.h"

int main (int argc, char *argv[]) {
int  i, valor[5], meu_ranque, raiz = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);

    
    if (meu_ranque == raiz) {
        for(i = 0; i < 5; i++){
            printf("Entre com o valor[%d]:\n", i);
            scanf("%d", &valor[i]);    
        }
    }

    MPI_Bcast(&valor, 5, MPI_INT, raiz, MPI_COMM_WORLD);

    
    for(i = 0; i < 5; i++){
        printf("O processo com ranque %d recebeu o valor[%d] %d\n",meu_ranque, i, valor[i]);
    }

    MPI_Finalize();
    return 0;
}