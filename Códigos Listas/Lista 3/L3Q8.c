#include<stdio.h>
#include<omp.h>

int main(int argc, char *argv[]){
    int i, n = 50, a = 2, z[50], x[50] = 3, y = 5;

    for (i = 0; i < n; i++)
        z[i] = a * x[i] + y;

    return 0;
}