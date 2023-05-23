#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define M 5
#define N 5
#define SZ 8

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];
int pos_buffer = 0;
pthread_mutex_t mutex;
pthread_cond_t cv;

void enviar(int *p)
{
	pthread_mutex_lock(&mutex);
	while (pos_buffer == SZ) {
		pthread_cond_wait(&cv, &mutex);
	}
	buffer[pos_buffer] = p;
	pos_buffer++;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cv);
	return;
}

int * recibir()
{
	int* ret;
	pthread_mutex_lock(&mutex);
	while (pos_buffer == 0) {
		pthread_cond_wait(&cv, &mutex);
	}
	pos_buffer--;
	ret = buffer[pos_buffer];
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cv);
	return ret;
}

void * prod_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);

		int *p = malloc(sizeof *p);
		*p = random() % 100;
		printf("Productor %d: produje %p->%d\n", id, p, *p);
		enviar(p);
	}
	return NULL;
}

void * cons_f(void *arg)
{
	int id = arg - (void*)0;
	while (1) {
		sleep(random() % 3);

		/* puts("Que mierda"); */
		int *p = recibir();
		printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);
		free(p);
	}
	return NULL;
}

int main()
{
	pthread_t productores[M], consumidores[N];
	int i;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cv, NULL);

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}
