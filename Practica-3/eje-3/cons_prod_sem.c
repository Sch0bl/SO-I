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

sem_t consumidores_sem; 
sem_t productores_sem;
sem_t pos_sem;

void enviar(int *p)
{
	sem_wait(&productores_sem); // Decrece el cupo de producción
	sem_wait(&pos_sem); // lock
	buffer[pos_buffer] = p;
	pos_buffer++;
	sem_post(&pos_sem); // unlock 
	sem_post(&consumidores_sem);// Habilita producto para consumir
	return;
}

int * recibir()
{
	int* ret;
	sem_wait(&consumidores_sem); // Decrece el cupo de productos
	sem_wait(&pos_sem); // lock
	pos_buffer--;
	ret = buffer[pos_buffer];
	sem_post(&pos_sem); // unlock
	sem_post(&productores_sem);// Habilita cupo para producir
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
	sem_init(&consumidores_sem,0,0);
	sem_init(&productores_sem,0,SZ);
	sem_init(&pos_sem,0,1); //Lock

	for (i = 0; i < M; i++)
		pthread_create(&productores[i], NULL, prod_f, i + (void*)0);

	for (i = 0; i < N; i++)
		pthread_create(&consumidores[i], NULL, cons_f, i + (void*)0);

	pthread_join(productores[0], NULL); /* Espera para siempre */
	return 0;
}
