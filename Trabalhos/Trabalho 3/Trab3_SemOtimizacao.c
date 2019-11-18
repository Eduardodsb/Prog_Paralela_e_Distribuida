/*
Compilar: gcc -o Trab3_SemOtimizacao Trab3_SemOtimizacao.c -fopenmp
Executar: ./Trab3_SemOtimizacao 1024
OBS: 1024 representa o tamanho da matriz.
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>

int main(int argc, char *argv[]){
    int i,j,k,N;
    double **a, **b, **c; 
    double t_final, t_inicial;

    srand(time(NULL));

    if(argc < 2){
        perror("Número de argumentos insuficiente, insira a dimensão da matriz quadrada");
        return 0;
    }else{
        N = atoi(argv[1]);
    }


    /*Alocação da Matrizes*/    
    a = (double**) malloc(sizeof(double*)*N);
    b = (double**) malloc(sizeof(double*)*N);
    c = (double**) malloc(sizeof(double*)*N);
    for(i=0; i<N; i++){
        a[i] = (double*) malloc(sizeof(double)*N);
        b[i] = (double*) malloc(sizeof(double)*N);
        c[i] = (double*) malloc(sizeof(double)*N);
    }

    /*Inicialização das matrizes*/
    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
            a[i][j] = rand()%N;
            b[i][j] = rand()%N;
            c[i][j] = 0.0;
        }
    }

    t_inicial = omp_get_wtime();

    for(i=0; i<N; i++)
        for (j=0; j<N; j++)
            for (k=0; k<N; k++)
                c[i][j] += a[i][k] * b[k][j];

    t_final = omp_get_wtime();


    printf("Tempo de execução: %lf\n", t_final-t_inicial);

    /*Libera memória utilizada*/
   for(i=0; i<N; i++){
		free(a[i]);
        free(b[i]);
        free(c[i]);
	}
    free(a);
    free(b);
    free(c);

    return 0;
}