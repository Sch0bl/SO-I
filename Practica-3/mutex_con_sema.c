#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
int contar = 0;

sem_t semaphore_mutex;

void* func(void* arg){
	/* region critica */
	for(int i = 0; i < 100000000; ++i){
		/* lock */
		sem_wait(&semaphore_mutex);
		contar++;
		printf("thread: %ld hola: %d\n",pthread_self(), contar);
		sem_post(&semaphore_mutex);
	}
	return NULL;
}

int main(){

	sem_init(&semaphore_mutex, 0, 1);
	pthread_t t1, t2;
	pthread_create(&t1,NULL,func, NULL);
	pthread_create(&t2,NULL,func, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("contar: %d\n", contar);


	return 0;
}
