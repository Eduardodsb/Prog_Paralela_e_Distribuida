/*
Compilar: gcc -o Q3 Q3.c -ansi -Wall -fopenmp -lm
Executar: ./Q3 4 10000
*/

#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void inicializa(int *vetor, int tam);
int encontraPrimos(int *vetor, int tam);

int main(int argc, char *argv[]){
    int *vetor, tam, totalPrimos;
    double t_final, t_inicial;

    if(argc < 3){
        perror("Número de argumentos insuficiente, insira a quantidade de threads e o limite superior (n) na busca por primos.");
        return 0;
    }else{
        omp_set_num_threads(atoi(argv[1]));
        tam = atoi(argv[2])+1;
        vetor = (int*) malloc(sizeof(int)*tam);
    }

    inicializa(vetor, tam);
    t_inicial = omp_get_wtime();
    totalPrimos = encontraPrimos(vetor, tam);
    t_final = omp_get_wtime();

    printf("Total de primos = %d\n", totalPrimos);
    printf("Tempo de execução: %lf\n", t_final-t_inicial);

    return 0;
}

void inicializa(int *vetor, int tam){
    int i;
    for(i=0;i<tam;i++){
        vetor[i] = 0;
    }
}

int encontraPrimos(int *vetor, int tam){
    int i, j, totalNaoPrimos = 0;
    for(i = 2; i < (int)(sqrt(tam) + 1);){
        for(j=i+1; j < tam; j++){
            if(j%i == 0){
                if(vetor[j] == 0)
                    totalNaoPrimos++;
                vetor[j] = 1;
            }
        }

        j=i+1;
        while(vetor[j] != 0)
            j++;
        i = j;
        
    }

    return tam - (totalNaoPrimos+2);
}