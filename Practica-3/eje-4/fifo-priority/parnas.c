#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "read_write.h"

#define M 5
#define N 5
#define ARRLEN 10240

struct_t st;

int arr[ARRLEN];
void * escritor(void *arg)
{
	int i;
	int num = arg - (void*)0;
	while (1) {
		sleep(random() % 2);
		write_lock(&st);
		printf("Escritor %d escribiendo\n", num);
		for (i = 0; i < ARRLEN; i++)
			arr[i] = num;
		write_unlock(&st);
	}
	return NULL;
}

void * lector(void *arg)
{
	int v, i;
	int num = arg - (void*)0;
	while (1) {
		sleep(random() % 2);
		read_lock(&st);
		v = arr[0];
		for (i = 1; i < ARRLEN; i++) {
			if (arr[i] != v)
				break;
		}
		if (i < ARRLEN)
			printf("Lector %d, error de lectura\n", num);
		else
			printf("Lector %d, dato %d\n", num, v);
		read_unlock(&st);
	}
	return NULL;
}

int main()
{
	pthread_t lectores[M], escritores[N];
	int i;

	rw_struct_init(&st);
	
	for (i = 0; i < M; i++)
		pthread_create(&lectores[i], NULL, lector, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&escritores[i], NULL, escritor, i + (void*)0);

	pthread_join(lectores[0], NULL); /* Espera para siempre */
return 0;
}
