#include <stdio.h>
#define SIZE 5000

float a[SIZE][SIZE];
float b[SIZE][SIZE];
float c[SIZE][SIZE];

int main() {
    int i,j,k;
    double t_inicial, t_final;

    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            a[i][j] = (float)i + j;
            b[i][j] = (float)i - j;
            c[i][j] = 0.0f;
        }
    }

    t_inicial = omp_get_wtime();
    #pragma acc data create(c) copyin(a,b)
    {
        //#pragma acc kernels
        #pragma acc parallel loop collapse(2) vector_length(128) tile(1000)
        for (i = 0; i < SIZE; ++i) {
            for (j = 0; j < SIZE; ++j) {
                c[i][j] = a[i][j] + b[i][j];
            }
        }
    }
    t_final = omp_get_wtime();
    
    return 0;
}