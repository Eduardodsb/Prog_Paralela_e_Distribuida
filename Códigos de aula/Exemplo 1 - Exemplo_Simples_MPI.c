#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[]) {

    int meu_ranque, num_procs, origem, destino, etiq=0;
    char mensagem[200];
    MPI_Status	info;

    MPI_Init(&argc, &argv); //Inicio da região de código paralelo.

    if (MPI_Initialized){ //"MPI_Initialized" retorna true caso "MPI_Init(&argc, &argv)" já tenha sido chamada. Além disso, "MPI_Initialized" também pode ser usado o seguinte modo: "MPI_Initialized(&iniciado)" onde o booleano será salvo na var iniciado.
        //OBS: MPI_Initialized é a única função que pode ser chamada antes do MPI_Init.
         printf("MPI iniciado! \n");
    }else{
        MPI_Init(&argc, &argv);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque); //Salva em "meu_ranque" o id do processo, dentro de um grupo. 
    MPI_Comm_size(MPI_COMM_WORLD,&num_procs); //Salva em "num_procs" a quantidade total de threads/processos de um mesmo grupo. 
	
    if (meu_ranque != 0)  { //Todos os processos que não seja o processo de id igual a 0.
        sprintf(mensagem, "Processo %d está vivo!", meu_ranque);
        destino = 0;
        MPI_Send(mensagem,strlen(mensagem)+1,MPI_CHAR,destino,etiq, MPI_COMM_WORLD); //Envia uma a mensagem "mensagem", de tamanho "strlen(mensagem)+1" e do tipo "MPI_CHAR" para o destino "destino", com a etiqueta "etiq" através do comunicador "MPI_COMM_WORLD".
   } else {
        for (origem=1; origem < num_procs; origem++) {
            MPI_Recv(mensagem, 200, MPI_CHAR, origem, etiq, MPI_COMM_WORLD, &info); //Recebe uma mensagem em "mensagem", de tamanho 200 e do tipo "MPI_CHAR" da origem "origem", com a etiqueta "etiq" através do comunicador MPI_COMM_WORLD. 
            //OBS1: origem e etiq podem ser susbtituidos por MPI_ANY_SOURCE e MPI_ANY_TAG que são coringas para aceitar de qualquer origem ou qualquer etiqueta. 
            //OBS2: "info" é uma estrutura com três elementos: MPI_SOURCE, MPI_TAG, MPI_ERROR.
            printf("%s\n",mensagem); 
        }
   } 

    MPI_Finalize(); //Fim da região de código paralelo

    if (MPI_Finalized){ //"MPI_Finalized" retorna true caso "MPI_Finalize()" já tenha sido chamada. Além disso, "MPI_Finalized" também pode ser usado o seguinte modo: "MPI_Finalized(&finalizado)" onde o booleano será salvo na var finalizado.
         printf("MPI finalizado! \n");
    }else{
        MPI_Finalize();
    }

    return 0;
}
