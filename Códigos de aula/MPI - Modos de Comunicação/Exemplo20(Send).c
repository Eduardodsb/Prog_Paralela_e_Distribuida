/*
Modo de comunicação: padrão
*/

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
int i, meu_ranque, num_procs;
int pot2, destino, meu_valor;
int reducao, recebido, etiq=1, cont=1;
MPI_Status estado;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    /* Aborta se o número de processos não for potência de 2 */
    pot2 = num_procs;
    while (((pot2 % 2) == 0) && pot2 > 1)
        pot2 /= 2;
    if (pot2 != 1) {
        if (meu_ranque == 0)
           printf("Por favor execute com número de processos potencia de 2 \n");
        MPI_Finalize();
        exit(0);
        }
    /* Cada processo tem um valor diferente */
    meu_valor = meu_ranque*num_procs;
    reducao = meu_valor;
    /* Realiza a troca de mensagens no padrão do algoritmo de "recursive doubling" */
    for (i = 1; i <= (num_procs/2); i += i) {
        if ((meu_ranque/i)%2 == 0) 
            destino = meu_ranque + i;
        else 
            destino = meu_ranque-i;
    /* Realiza as operações de envio primeiro. Vai na fé! */
        MPI_Send(&reducao, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD);
    /* Realiza as operações de recepção. Se tudo deu certo, consegue chegar aqui. */
        MPI_Recv(&recebido, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD, &estado);
    /* Realiza a operação de redução */
        if (recebido > reducao)
                reducao = recebido;
    }
    printf("Meu valor = %d, redução = %d \n", meu_valor, reducao);
    MPI_Finalize();
    return 0;
}