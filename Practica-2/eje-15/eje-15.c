#include <stdio.h>
#include <pthread.h>
#include <math.h>

#define N 10
#define VPM 100

int visitantes = 0;

int flag[N];
int choose[N]; // choose[i] == 1 si i esta eligiendo maximo

int max(int* v) {
  int result = v[0];
	for (int i = 1; i < N; i++)
		if (v[i] > result)
			result = v[i];
	return result;
}

void* panaderia(void* arg){
	int id = *(int*) arg;
  printf("id: %d\n", id);
	// Calcular numero de turno
	for (int n = 0; n < VPM; n++) {
	  choose[id] = 1;
    flag[id] = 1 + max(flag);
	  choose[id] = 0;

	  for (int j = 0; j < N; j++) {
	  	while (choose[j]);
	  	while (flag[j] != 0 && flag[j] < flag[id]);
  	}
  	visitantes = visitantes + 1;	
  	flag[id] = 0;
	}
	return NULL;
}


int main(){
  pthread_t t[N];
	int id[N];
	for (int i = 0; i < N; i++) {
    choose[i] = 0;
		id[i] = i;
		flag[i] = 0;
	}

  for (int i = 0; i < N; i++)
		pthread_create(t + i, NULL, panaderia, (void*) (id + i));
	for (int i = 0; i < N; i++)
		pthread_join(t[i], NULL);

  printf("Visitantes: %d\n", visitantes);

	return 0;
}
