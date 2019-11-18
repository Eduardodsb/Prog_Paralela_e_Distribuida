/*
Compilar: gcc -o Trab3_Otimizado Trab3_Otimizado.c -fopenmp -O3
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>

#define N 1024

int main(int argc, char *argv[]){
    int i,j,k;
    double *a, *b, *c; 
    double t_final, t_inicial;
    int sum = 0; /*Variável temporária para manter em registrador o valor acumulado para cálculo de C[i][j]*/

    srand(time(NULL));

    /*Alocação da Matrizes*/    
    a = (double*) malloc(sizeof(double)*N*N);
    b = (double*) malloc(sizeof(double)*N*N);
    c = (double*) malloc(sizeof(double)*N*N);

    /*Inicialização das matrizes*/
    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
            a[i*N+j] = rand()%N;
            b[i*N+j] = rand()%N;
            c[i*N+j] = 0.0;
        }
    }
    
    /*OBS: B deve ser transposta*/
    
    t_inicial = omp_get_wtime();

    for(i=0; i<N; i++){
        for (j=0; j<N; j++){
            for (k=0; k<N; k++){
                //sum += a[i*N+k] * b[k*N+j];
                //sum += *(a + (i*N+k)) * *(b + (k*N+j)); /*Para b não transposto*/
                sum += *(a + (i*N+k)) * *(b + (j*N+k)); /*Para b transposto*/
            }
            //c[i*N+j] = sum;
            *(c+(i*N+j)) = sum;
        }
    }   
    t_final = omp_get_wtime();


    printf("Tempo de execução: %lf\n", t_final-t_inicial);

    /*Libera memória utilizada*/
    free(a);
    free(b);
    free(c);

    return 0;
}