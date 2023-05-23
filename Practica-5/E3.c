#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define N (14969 * 2)

int main(){
    int id;
    int j = floor(sqrt(N));
    int flag = 0; 
    #pragma omp parallel for
    for(int i = 2; i <= j; i++)
    {
        if(flag) i = N+1;
        else if (N % i == 0){
            printf("No es primo\n");
            printf("i = %d j = %d\n", i, j);
            printf("Este es el maldito id: %d\n", omp_get_thread_num());
            #pragma omp critical
            flag = 1;
        }
    }
    
    return 0;
}