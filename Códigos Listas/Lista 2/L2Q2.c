#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[]) {

    int meu_ranque, num_procs, comprimento, versao, subversao;
    char nome_proces[MPI_MAX_PROCESSOR_NAME];
    int erro;

    if(MPI_Init(&argc, &argv) != MPI_SUCCESS){
        printf("Erro ao iniciar. O programa será abortado.\n");
        MPI_Abort(MPI_COMM_WORLD,erro);
    }

    printf("Size int %ld\n", sizeof(int));
    printf("Size int %ld\n", sizeof(__int32_t));


    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Get_processor_name(nome_proces, &comprimento);
    MPI_Get_version (&versao, &subversao);

    if(meu_ranque == 0)
        printf("Existem %d processos. Versão MPI: %d | SubVersão MPI: %d\n", num_procs, versao, subversao);

    printf("Meu ranque: %d | Meu nome: %s | Tamanho do nome: %d\n", meu_ranque, nome_proces, comprimento);

    MPI_Finalize();
    return 0;
}