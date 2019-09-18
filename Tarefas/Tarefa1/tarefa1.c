#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

void mxv(int n, double *A, double *b, double *c);
void imprimeMatriz(int m, int n, double *A);
void imprimeVetor(int m, double *c);
void imprimeVetorTransposto(int m, double *c);

int main(int argc, char *argv[]){
int i, j, m, n, meu_ranque, num_procs, raiz = 0;
double *A,*b,*c;

/*
A = Matriz com tamanho M x N.
b = Vetor de tamanho N, que multipĺicará a matriz.
c = Vetor que guardará o resultado da multiplicação da matriz A com o vetor transposto b. 
*/

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    m = num_procs;
    /*OBS: O número de linhas será a quantidade de processos.*/
    if(meu_ranque == 0){
        printf("Por favor entre com n° de colunas (n):\n");
        scanf("%d",&n);printf("\n");

        if ( (A=(double *)malloc(m*n*sizeof(double))) == NULL )
            perror("Erro de alocação de memória para a matriz A");
        if ( (b=(double *)malloc(n*sizeof(double))) == NULL )
            perror("Erro de alocação de memória para o vetor b");
        if ( (c=(double *)malloc(m*sizeof(double))) == NULL )
            perror("Erro de alocação de memória para o vetor c");

        printf("Atribuindo valor inicial à matriz A e ao vetor b\n");
        for (j=0; j<n; j++)
            b[j] = 2.0;
        for (i=0; i<m; i++){
            for (j=0; j<n; j++){
                A[i*n + j] = i;
            }
        }

        printf("A = ");
        imprimeMatriz(m, n, A);
        puts("");
        printf("b = ");
        imprimeVetorTransposto(n, b);
    }

    MPI_Bcast(&n, 1, MPI_INT, raiz, MPI_COMM_WORLD);

    if(meu_ranque != 0){
        if ( (A=(double *)malloc(n*sizeof(double))) == NULL )
            perror("Erro de alocação de memória para a matriz A");
        if ( (b=(double *)malloc(n*sizeof(double))) == NULL )
            perror("Erro de alocação de memória para o vetor b");
        if ( (c=(double *)malloc(sizeof(double))) == NULL )
            perror("Erro de alocação de memória para o vetor c");
    }
    
    MPI_Bcast(b, n, MPI_DOUBLE, raiz, MPI_COMM_WORLD);
    MPI_Scatter(A, n, MPI_DOUBLE, A, n, MPI_DOUBLE, raiz, MPI_COMM_WORLD);

    puts("");
    printf("Mutiplicando a matriz A com o vetor b\n");
    (void) mxv(n, A, b, c);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(c, 1, MPI_DOUBLE, c, 1, MPI_DOUBLE, raiz, MPI_COMM_WORLD);

    if(meu_ranque == 0){
        puts("");
        printf("c = ");
        imprimeVetor(m, c);
    }

    free(A);
    free(b);
    free(c);

    MPI_Finalize();
    return(0);
}

void mxv(int n, double *A, double *b, double *c){
    int i;
    for (i=0; i<n; i++)
        c[0] += A[i]*b[i];
    
}

void imprimeMatriz(int m, int n, double *A){
    int i, j;
    printf("[\n");
    for (i=0; i<m; i++){
        for (j=0; j<n; j++){
            A[i*n + j] = i;
            printf("%lf ", A[i*n + j]);
        }
        printf("\n");
    }
    printf("]\n");
}

void imprimeVetor(int m, double *c){
    int i;
    printf("[ ");
    for(i=0; i < m; i++)
        printf("%lf ", c[i]);
    printf("]\n"); 
}

void imprimeVetorTransposto(int m, double *c){
    int i;
    printf("[\n");
    for(i=0; i < m; i++)
        printf("%lf\n", c[i]);
    printf("]\n"); 
}