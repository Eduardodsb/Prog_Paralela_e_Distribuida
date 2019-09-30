/*
Ordena Vetor - Código Paralelo.

Compilar: mpicc -o ordena_para ordena_para.c -lm
Executar: mpirun -np 8 ./ordena_para 16000000

OBS1: O número de processos deve ser potência de 2.
OBS2: O argumento pode ser alterado, o mesmo representa o tamanho do vetor.  
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"
#include <time.h>

void merge(int *vetor, int comeco, int meio, int fim);
void mergeSort(int *vetor, int comeco, int fim);
void balanceamentoCarga(int *deslocamento, int * tamVetorLocal, int tamVetor, int num_procs);

int main(int argc, char **argv) {
int i, j, meu_ranque, num_procs, raiz = 0, etiq = 1;
int *vetor, tamVetor, *vetorLocal, *tamVetorLocal, *vetorTemp, tamVetorTemp, *deslocamento; /*deslocamento é utilizada para auxiliar o uso do MPI_Scatterv*/
double t_inicial, t_final; /*Variáveis utilizadas para cálculo de tempo de execução*/
int filhoEsquerda, filhoDireita;  /*Variáveis utilizadas para navegar na árvore*/
MPI_Status	info;
srand( time(NULL) );

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
     
    tamVetorLocal = (int *)malloc(num_procs*sizeof(int)); /*Guardará o tamanho do vetor local de cada processo*/
    
    if(meu_ranque == raiz){
        //Verifica quantidade de argumentos
        if(argc != 2){
            printf("Número incorreto de argumentos\n");
            exit(-1);
        }
        //Ler argumento e alloca espaço para o array. 
        sscanf(argv[1], "%d", &tamVetor);
        if((vetor = (int*) malloc(sizeof(int)*tamVetor)) == NULL){
            printf("Erro ao alocar o vetor na memória\n");
            exit(-1);
        }

        //Gerando vetor aleatório.
        printf("Gerando vetor com números aleatórios...\n");
        for(i = 0; i < tamVetor; i++){
            vetor[i] = rand();
            //printf("- %d ", vetor[i]);
        } //puts("");       

        //Balanceamento de carga
        printf("Balanceando carga...\n");
        deslocamento = (int *)malloc(num_procs*sizeof(int));
        balanceamentoCarga(deslocamento, tamVetorLocal, tamVetor, num_procs);

        printf("Iniciando a ordenação!\n");
    }

    //Comunicação - Informa aos processos o tamanho dos seus vetores locais e trasmite o pedaço de cada processo. 
    MPI_Bcast(tamVetorLocal, num_procs, MPI_INT, raiz, MPI_COMM_WORLD);
    vetorLocal = (int*) malloc(tamVetorLocal[meu_ranque]*sizeof(int));
    MPI_Scatterv(vetor, tamVetorLocal, deslocamento, MPI_INT, vetorLocal, tamVetorLocal[meu_ranque], MPI_INT, raiz, MPI_COMM_WORLD);

    //Inicia a contagem do tempo de execução da ordenação.
    t_inicial = MPI_Wtime();

    //Cada processo fará Merge Sort do pedaço recebido (Sort local).
    mergeSort(vetorLocal, 0, tamVetorLocal[meu_ranque]-1);
    printf("Eu processo %d saí do mergeSort local\n", meu_ranque);

    //Os processo a direita na ávore deve enviar os vetores ordenados para o processo irmão a esquerda.
    for(i = 0; i < (int)log2(num_procs); i++){ //Log2(num_procs) é a altura da árvore.
        filhoEsquerda = (meu_ranque & (~(1<<i)));
        filhoDireita = meu_ranque|(1<<i);
        
        if(meu_ranque == filhoEsquerda ){ //Filho da esquerda recebe o vetor do filho da direita para fazer o merge
            MPI_Probe(filhoDireita, etiq, MPI_COMM_WORLD, &info); 
            MPI_Get_count(&info, MPI_INT, &tamVetorTemp);

            tamVetorTemp += tamVetorLocal[meu_ranque];
            vetorTemp = (int*) malloc(sizeof(int)*tamVetorTemp);

            MPI_Recv(vetorTemp, tamVetorTemp, MPI_INT, filhoDireita, etiq, MPI_COMM_WORLD, &info);

            for(j = tamVetorTemp-tamVetorLocal[meu_ranque]; j < tamVetorTemp; j++){ //Concatenar o vetor recebido com o seu.
                vetorTemp[j] = vetorLocal[j-(tamVetorTemp-tamVetorLocal[meu_ranque])];
            } 

            merge(vetorTemp,0,tamVetorTemp-tamVetorLocal[meu_ranque]-1,tamVetorTemp-1); //Reordena esse novo vetor
            free(vetorLocal);
            vetorLocal = vetorTemp;
            tamVetorLocal[meu_ranque] = tamVetorTemp;

        }else{ //Filho da direita envia seu vetor local, libera a memória e finaliza
             MPI_Send(vetorLocal,tamVetorLocal[meu_ranque],MPI_INT,filhoEsquerda,etiq, MPI_COMM_WORLD); 
             free(vetorLocal);
             i = log2(num_procs);
        }

    }

    //Finaliza a contagem do tempo de execução da ordenação. OBS: O tempo considerado é o do processo 0, pois o mesmo é o processo raiz da árvore. 
    t_final = MPI_Wtime();

    printf("Eu sou o processo %d e estou finalizando.\n", meu_ranque);
    
    if(meu_ranque == raiz){
        free(vetor);
        /*for(i = 0; i<tamVetorLocal[meu_ranque]; i++){
            printf("- %d ", vetorLocal[i]);
        }*/
        puts("");
        free(vetorLocal);
        printf ("Tempo de execução %.5f\n", t_final - t_inicial);
    }
    MPI_Finalize();
    return 0;
}

void mergeSort(int *vetor, int comeco, int fim){
int meio;
    if(comeco < fim){
        meio = (comeco + fim)/2;
        mergeSort(vetor, comeco, meio);
        mergeSort(vetor, meio+1, fim);
        merge(vetor, comeco, meio, fim);
    }

}

void merge(int *vetor, int comeco, int meio, int fim){
int metade1 = comeco, metade2 = meio+1;
int *vetorOrdenado, comecoOrdenado = 0;

    vetorOrdenado = (int*) malloc(sizeof(int)*(fim-comeco+1));

    while(metade1 <= meio && metade2 <= fim){ /*Compoe o vetorOrdenado olhando termo a termo das duas metades do vetor, que se deseja ordenar.*/
        if(vetor[metade1] < vetor[metade2]){
            vetorOrdenado[comecoOrdenado] = vetor[metade1];
            metade1++;
        }else{
            vetorOrdenado[comecoOrdenado] = vetor[metade2];
            metade2++;
        }
        comecoOrdenado++;
    }

    while(metade1 <= meio){ /*Pega os termos restantes da primeira metade do vetor.*/
        vetorOrdenado[comecoOrdenado] = vetor[metade1];
        metade1++;
        comecoOrdenado++;
    }

    while(metade2 <= fim){ /*Pega os termos restantes da segunda metade do vetor.*/
        vetorOrdenado[comecoOrdenado] = vetor[metade2];
        metade2++;
        comecoOrdenado++;
    }

    /*Atribui ao vetor original o endereço que contem o vetor ordenado.*/

    for(comecoOrdenado = comeco; comecoOrdenado <= fim; comecoOrdenado++){ 
        vetor[comecoOrdenado] = vetorOrdenado[comecoOrdenado-comeco];
    }

    free(vetorOrdenado);
}

void balanceamentoCarga(int *deslocamento, int * tamVetorLocal, int tamVetor, int num_procs){
int tam, i, salto = 0, sobra;
    tam = floor((double)tamVetor/num_procs);
    sobra = tamVetor - tam*num_procs;

    for(i = 0; i<num_procs; i++){
        tamVetorLocal[i] = tam;
        if(sobra>0){
            tamVetorLocal[i]++;
            sobra--;
        }
        deslocamento[i] = salto;
        salto += tamVetorLocal[i];
    }
}