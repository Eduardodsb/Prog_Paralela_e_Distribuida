#include <stdio.h>
#include "mpi.h"

int main (int argc, char *argv[]) {
int meu_ranque, num_procs; 
int versao, subversao,  aux, ret;
char maquina[MPI_MAX_PROCESSOR_NAME];

    /* Inicia o MPI. Em caso de erro aborta o programa */
    ret = MPI_Init(&argc, &argv);
    if (ret != MPI_SUCCESS) {
        printf("Erro ao iniciar o programa MPI. Terminando.\n");
        MPI_Abort(MPI_COMM_WORLD, ret);
    }

    /* Imprime a versão e subversão da biblioteca MPI em uso */
    MPI_Get_version(&versao,&subversao);
    printf("Versão do MPI  = %d Subversão = %d \n", versao, subversao);

    /* Determina o ranque e o número de processos em execução */
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);

    /* Determina o nome do computador onde o processo está executando */
    MPI_Get_processor_name(maquina, &aux);
    printf("Número de tarefas = %d Meu ranque = %d Executando em  %s\n", num_procs, meu_ranque, maquina);
    
    /* Finaliza o MPI */
    MPI_Finalize();
}
