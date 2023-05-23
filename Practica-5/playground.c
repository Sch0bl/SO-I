#include <omp.h>
#include <stdio.h>

int a[1000000];

int main(){
#pragma omp parallel
{
  #pragma omp for
  for (int i = 0; i < 1000000; i++){
    a[i] = 2 * i;
  }
}

int i = 10;
#pragma omp parallel private(i)
{
  // variable i is not initialized
  printf("thread %d: i = %d\n", omp_get_thread_num(), i);
  i = 1000;
}
  printf("private i = %d\n", i);
#pragma omp parallel firstprivate(i)
{
  // variable i is not initialized
  printf("thread %d: i = %d\n", omp_get_thread_num(), i);
  i = 1000;
}
  printf("private i = %d\n", i);
  return 0;
}
