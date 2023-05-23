#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
sem_t tabaco, papel, fosforos, otra_vez, prioridad;
void agente()
{
	while (1) {
		sem_wait(&otra_vez); //Espera a que haya terminado de fumar 
		int caso = random() % 3; //Elige un ingrediente al azar
		if (caso != 0) sem_post(&fosforos);
		if (caso != 1) sem_post(&papel);
		if (caso != 2) sem_post(&tabaco);
	}
}
void fumar(int fumador)
{
	printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
	sleep(1);
}


//El problema que se presenta en la siguiente obra de arte
//es que los procesos no estan sincronizados, por lo cual si
//estan disponibles los elementos tabaco y fosforos, el fumador2
//debería tener el derecho a drogarse, pero el adicto del fumador1,
//se adelante por su síndrome de abstinencia y toma el tabaco, el fumador 2, toma los foosforos y todos los semáforos quedan en 0, produ
//ciendo el deadlock, ya que quedan todos los fumadores esperando 
//los elementos
void * fumador1(void *arg) // Elemento igneo
{
	while (1) {
		sem_wait(&tabaco);
		if(!sem_trywait(&papel)){
			sem_post(&tabaco);
			continue;
		}
		fumar(1);
		sem_post(&otra_vez);
	}
}
void * fumador2(void *arg)// Liyo
{
	while (1) {
		sem_wait(&fosforos);
		if(!sem_trywait(&tabaco)){
			sem_post(&tabaco);
			continue;
		}
		fumar(2);
		sem_post(&otra_vez);
	}
}
void * fumador3(void *arg){// Marimba
	while (1) {
		sem_wait(&papel);
		if(!sem_trywait(&fosforos)){
			sem_post(&tabaco);
			continue;
		}
		fumar(3);
		sem_post(&otra_vez);
	}
}
int main()
{
	pthread_t s1, s2, s3;
	sem_init(&tabaco, 0, 0);
	sem_init(&papel, 0, 0);
	sem_init(&fosforos, 0, 0);
	sem_init(&otra_vez, 0, 1);
	sem_init(&prioridad, 0, 0);
	pthread_create(&s1, NULL, fumador1, NULL);
	pthread_create(&s2, NULL, fumador2, NULL);
	pthread_create(&s3, NULL, fumador3, NULL);
	agente();
	return 0;
}
