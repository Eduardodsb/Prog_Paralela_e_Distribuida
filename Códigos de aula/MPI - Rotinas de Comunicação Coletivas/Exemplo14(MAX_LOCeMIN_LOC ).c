/*Para estudar esse código é necessário rodar com 12 processos*/
/*mpirun -np 12 ./Exemplo14\(MAX_LOCeMIN_LOC\ \)*/

#include <stdio.h>
#include "mpi.h"
#define TAM 12

int main (int argc, char *argv[]){
int meu_ranque, num_procs;/* O ranque e o número de processos */
int i, raiz = 0; /* Todas as mensagens vão para 0 */
int indice[TAM];
double vetor_entrada[TAM ]={0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,12.0};
double vetor_saida[TAM];

struct {
double valor;
int ranque;
} entrada[TAM], saida[TAM];

        MPI_Status estado;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
        /* Só os processos com ranques pares colocam valor 50 no
        vetor */

        if ((meu_ranque%2) == 0)
            vetor_entrada[meu_ranque] = 50.0;
        
        /*Prepara os parametros de entrada função de redução */
        for (i = 0; i < TAM; ++i) {
            entrada[i].valor = vetor_entrada[i];
            entrada[i].ranque = meu_ranque;
        }

        /* Faz a operação de redução utilizando o tipo especial MPI_DOUBLE_INT */
        MPI_Reduce(entrada, saida, TAM, MPI_DOUBLE_INT, MPI_MAXLOC, raiz, MPI_COMM_WORLD);
        
        /* O processo raiz imprime o resultado da operação de redução */
        if (meu_ranque == raiz) {
            for (i = 0; i < TAM; ++i) {
                vetor_saida[i] = saida[i].valor;
                indice[i] = saida[i].ranque;
                printf("Resultado = %3.1f Processo = %d \n",
                vetor_saida[i], indice[i]);
            }
        }
        
        /* Termina a execução */
        MPI_Finalize();
}