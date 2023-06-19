#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include "../timing.h"
#include <math.h>

#define N 1000
int A[N][N], B[N][N], C[N][N];

int mult_par(int A[N][N],int B[N][N], int C[N][N]) {
  int i, j, k;
  #pragma omp parallel for private(j,k)
  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++)
      for(k = 0; k < N; k++)
        C[i][j] += A[i][k] * B[j][k];

  return 0;
}


int mult_par_ind_1(int A[N][N],int B[N][N], int C[N][N]) {
  int i, j, k;
  #pragma omp parallel for private(j,k)
  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++)
      for(k = 0; k < N; k++)
        C[i][k] += A[i][j] * B[j][k];

  return 0;
}


int mult_par_ind_2(int A[N][N],int B[N][N], int C[N][N]) {
  int i, j, k;
  #pragma omp parallel for private(i,k)
  for(j = 0; j < N; j++)
    for(i = 0; i < N; i++)
      for(k = 0; k < N; k++)
        C[i][j] += A[i][k] * B[k][j];

  return 0;
}

int mult_sec(int A[N][N],int B[N][N], int C[N][N]) {
  int i, j, k;

  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++)
      for(k = 0; k < N; k++)
        C[i][j] += A[i][k] * B[k][j];

  return 0;
}

int main(){
  int i, j;  

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      A[i][j] = random() % 1000;
      B[i][j] = random() % 1000;
    }
  }

  TIME(mult_par(A,B,C), NULL);
  TIME(mult_par_ind_1(A,B,C), NULL);
  TIME(mult_par_ind_2(A,B,C), NULL);
  TIME(mult_sec(A,B,C), NULL);

  return 0;
}
