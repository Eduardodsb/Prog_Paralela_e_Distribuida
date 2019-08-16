/*
Possui a mesma ideia do código do exemplo3.Contúdo, nesse código a variável qua guardará a msg recebida é alocada dinâmicamente com o auxílio da função MPI_Probe.
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 100

int main (int argc, char *argv[]) {
int  meu_ranque, num_procs, etiq=0;
int* vet_num;
int total_num, origem=0, destino=1, numeros[MAX];
MPI_Status estado;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    /* Envia um número aleatório de dados */
    if (meu_ranque == origem) {
       
        srand(MPI_Wtime());
        total_num = (rand() / (float)RAND_MAX) * MAX;
        MPI_Send(numeros, total_num, MPI_INT, destino, etiq, MPI_COMM_WORLD);
        printf("Processo %d enviou %d números  para processo %d.\n", origem, total_num, destino);
    
    }else if (meu_ranque == destino) {

    /* Testa para uma mensagem chegando do processo origem */
        MPI_Probe(origem, etiq, MPI_COMM_WORLD, &estado); // "MPI_Probe" fica aguardando chegar uma msg da origem "origem" com a etiqueta "etiq" e salva os status da mesma em "estado".
    /* Quando a rotina MPI_Probe retorna, o manipulador "estado" contém
       o tamanho da mensagem que chega. Obtém o tamanho da mensagem */
        MPI_Get_count(&estado, MPI_INT, &total_num);
    /* Aloca um vetor para guardar os números transmitidos */
        vet_num = (int*) malloc(sizeof(int)*total_num);
    /* Agora recebe a mensagem com o vetor previamente alocado  */
        MPI_Recv(vet_num, total_num, MPI_INT, origem, etiq, MPI_COMM_WORLD, MPI_STATUS_IGNORE);//"MPI_STATUS_IGNORE" ignora o status. 
        printf("Processo %d recebeu dinamicamente %d números de %d.\n", destino, total_num, origem);
        free(vet_num);
    
    }

    MPI_Finalize();
    
    return 0;
}
