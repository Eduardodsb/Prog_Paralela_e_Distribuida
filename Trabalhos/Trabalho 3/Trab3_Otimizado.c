/*
Compilar gcc: gcc -o Trab3_Otimizado Trab3_Otimizado.c -fopenmp -O3
Compilar PGI: pgcc -mp -Minfo=all -o Trab3_Otimizado Trab3_Otimizado.c -O3
Compilar icc: icc -o Trab3_Otimizado Trab3_Otimizado.c -openmp
Executar: ./Trab3_Otimizado 2000 1000
OBS: 2000 representa o tamanho da matriz quadrada e o 1000 Block Size.
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>

int main(int argc, char *argv[]){
    int i,j,k, N, BLOCK_SIZE, m,r,p;
    double *a, *b, *c; 
    double t_final, t_inicial;
    double sum = 0; /*Variável temporária para manter em registrador o valor acumulado para cálculo de C[i][j]*/
    int num;

    srand(time(NULL));

    if(argc < 3){
        perror("Número de argumentos insuficiente, insira a dimensão da matriz quadrada e o block size");
        return 0;
    }else{
        N = atoi(argv[1]);
        BLOCK_SIZE = atoi(argv[2]);
    }

    num = N/BLOCK_SIZE;

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

    for(i=0; i<num; i++){
        for (j=0; j<num; j++){
            for (k=0; k<BLOCK_SIZE; k++){
                for(m=0; m<BLOCK_SIZE; m++){
                    sum = 0.0;
                    for(r=0;r<num;r++){
                        for(p=0; p<BLOCK_SIZE; p++){
                            //sum += a[i*N+k] * b[k*N+j];
                            //sum += *(a + (i*N+k)) * *(b + (k*N+j)); /*Para b não transposto*/
                            //sum += *(a + (i*N+k)) * *(b + (j*N+k)); /*Para b transposto*/
                            sum += *(a + (i*BLOCK_SIZE*N+ r*BLOCK_SIZE +k*N + p)) * *(b + (j*BLOCK_SIZE*N+r*BLOCK_SIZE+m*N+p)); /*Blocagem*/
                        }
                    }
                    *(c+(i*BLOCK_SIZE*N+j*BLOCK_SIZE+k*N+m)) = sum;
                }
            }
            //c[i*N+j] = sum;
            //*(c+(i*N+j)) = sum;
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