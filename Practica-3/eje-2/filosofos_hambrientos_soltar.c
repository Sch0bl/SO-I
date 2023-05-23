#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N_FILOSOFOS 2 
#define ESPERA 50

int cant_comidas = 0;

pthread_mutex_t tenedor[N_FILOSOFOS];

pthread_mutex_t * izq(int i) { 
	return &tenedor[i];
}

pthread_mutex_t * der(int i) { 
	return &tenedor[(i+1) % N_FILOSOFOS];
}

void pensar(int i) {
	/* printf("Filosofo %d pensando...\n", i); */
	usleep(random() % ESPERA);
}
void comer(int i)
{
	/* printf("Filosofo %d comiendo...\n", i); */
	usleep(random() % ESPERA);
}
void tomar_tenedores(int i)
{
	pthread_mutex_lock(der(i));
	while (pthread_mutex_trylock(izq(i))) {
		pthread_mutex_unlock(der(i));	//Teóricamente, puede existir un 
																	//livelock, si todos sueltan y agarran
																	//al mismo tiempo
		sleep(1);
		pthread_mutex_lock(der(i));
	}
}
void dejar_tenedores(int i)
{
	pthread_mutex_unlock(der(i));
	pthread_mutex_unlock(izq(i));
}
void * filosofo(void *arg)
{
	int i = arg - (void*)0;
	while (cant_comidas < 1000) {
		tomar_tenedores(i);
		comer(i);
		cant_comidas++;
		/* printf("Cantidad de comida: %d\n", cant_comidas); */
		dejar_tenedores(i);
		pensar(i);
	}
	return NULL;
}

int main()
{
	pthread_t filo[N_FILOSOFOS];
	int i;
	for (i = 0; i < N_FILOSOFOS; i++)
		pthread_mutex_init(&tenedor[i], NULL);
	for (i = 0; i < N_FILOSOFOS; i++)
		pthread_create(&filo[i], NULL, filosofo, i + (void*)0);
	pthread_join(filo[0], NULL);
	pthread_join(filo[1], NULL);
	printf("No me trabe.\n");
return 0;
}
