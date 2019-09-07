#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    int meu_ranque, num_procs;
    char resposta;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD,&meu_ranque);
    
    if (meu_ranque == 0 ) {
        printf ("Estou atrasado para a barreira! \n");
        resposta = getchar();
    }

    if(meu_ranque != 0 ){
        MPI_Barrier(MPI_COMM_WORLD);/*Todos os processos são retidos nesse ponto, até que todos cheguem processos cheguem aqui*/
        printf("Passei da barreira. Eu sou o %d de %d processos \n", meu_ranque, num_procs);
    }else{
        MPI_Barrier(MPI_COMM_WORLD);/*Obs: não precissa ser a mesma barreira, para liberar os demais processos.*/
        printf("Passei da barreira. Eu sou o %d de %d processos (Cheguei!!!) \n", meu_ranque, num_procs);
    }
   /*printf("Passei da barreira. Eu sou o %d de %d processos \n", meu_ranque, num_procs);*/
    fflush(stdout);
    MPI_Finalize();

    return 0;
}
