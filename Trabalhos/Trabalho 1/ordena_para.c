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

int main(int argc, char **argv) {
int i, j, meu_ranque, num_procs, raiz = 0, etiq = 1;
int *vetor, tamVetor, *vetorLocal, tamVetorLocal, *vetorTemp, tamVetorTemp;
double t_inicial, t_final;
int filhoEsquerda, filhoDireita; 
srand( time(NULL) );
MPI_Status	info;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if(meu_ranque == raiz){
        if(argc != 2){
            printf("Número incorreto de argumentos\n");
            exit(-1);
        }
        sscanf(argv[1], "%d", &tamVetor);
        if((vetor = (int*) malloc(sizeof(int)*tamVetor)) == NULL){
            printf("Erro ao alocar o vetor na memória\n");
            exit(-1);
        }

        //Gerando vetor aleatório.
        for(i = 0; i < tamVetor; i++){
            vetor[i] = rand();
            printf("%d, ", vetor[i]);
        }
        puts("");

        tamVetorLocal = tamVetor/num_procs;
    }

    MPI_Bcast(&tamVetorLocal, 1, MPI_INT, raiz, MPI_COMM_WORLD);
    vetorLocal = (int*) malloc(sizeof(int)*tamVetorLocal);
    MPI_Scatter(vetor, tamVetorLocal, MPI_INT, vetorLocal, tamVetorLocal, MPI_INT, raiz, MPI_COMM_WORLD);
    
    t_inicial = MPI_Wtime();

    //Cada processo fará merge sort do pedaço recebido (Sort local).
    mergeSort(vetorLocal, 0, tamVetorLocal-1);

    for(i = 0; i < (int)log2(num_procs); i++){ //Log2(num_procs) é a altura da árvore.
        filhoEsquerda = (meu_ranque & (~(1<<i)));
        filhoDireita = meu_ranque|(1<<i),etiq;
        
        if(meu_ranque == filhoEsquerda ){ /*Filho da esquerda recebe o vetor do filho da direita para fazer o merge*/
            tamVetorTemp = tamVetorLocal;
            vetorTemp = (int*) malloc(sizeof(int)*tamVetorTemp*2);
            MPI_Recv(vetorTemp, tamVetorTemp*2, MPI_INT, filhoDireita, etiq, MPI_COMM_WORLD, &info);
            for(j = tamVetorTemp; j < tamVetorTemp*2; j++)
                vetorTemp[j] = vetorLocal[j-tamVetorTemp]; 
            merge(vetorTemp,0,tamVetorTemp-1,tamVetorTemp*2-1);
            free(vetorLocal);
            vetorLocal = vetorTemp;
            tamVetorLocal = tamVetorTemp*2;
            //printf("%d\n", i);
        }else{ /*Filho da direita envia seu vetor local, libera a memória e finaliza*/
             MPI_Send(vetorLocal,tamVetorLocal,MPI_INT,filhoEsquerda,etiq, MPI_COMM_WORLD); 
             free(vetorLocal);
             i = log2(num_procs);
        }

    }

    t_final = MPI_Wtime();

    printf("Eu sou o processo %d e estou finalizando.\n", meu_ranque);
    
    if(meu_ranque == raiz){
        free(vetor);
        for(i = 0; i<tamVetorLocal; i++){
            printf("%d, ", vetorLocal[i]);
        }
        puts("");
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