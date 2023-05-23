#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 10000
int a[N];

int main(){
    srand(time(NULL)); 
    for(int i = 0; si < N; i++)
        a[i] = rand() % N;
    int min = a[0];
    
    #pragma omp parallel
    {
        #pragma omp for 
        for(int i = 0; i < N; i++){
            #pragma omp critical ( critical_section )
            {
                if (min > a[i])
                    min = a[i]; 
            }    
        }

    }
    printf("El minimo es: %d\n", min);
    return 0;
}