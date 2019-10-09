#include <stdio.h>  
#include <omp.h>  
 
int main() {  
   printf("Fora = %d\n", omp_in_parallel( )); 
   #pragma omp parallel num_threads(4)  
   {  
      int i = omp_get_thread_num(); 
      printf("Olá da thread %d\n", i);
      printf("Dentro = %d\n", omp_in_parallel( )); 
   }  
}
