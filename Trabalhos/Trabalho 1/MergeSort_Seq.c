#include<stdio.h>
#include<stdlib.h>
#include "mpi.h"
#include<time.h>

void merge(int *vetor, int comeco, int meio, int fim);
void mergeSort(int *vetor, int comeco, int fim);

int main(int argc, char const *argv[]) {
int i, *vetor, tamVetor;
double t_inicial, t_final;

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
    }

    t_inicial = MPI_Wtime();
    mergeSort(vetor,0,tamVetor-1);
    t_final = MPI_Wtime();

    printf ("Tempo de execução %.5f\n", t_final - t_inicial);

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
int *vetorOrdenado, comecoOrdenado = 0, *temp;

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