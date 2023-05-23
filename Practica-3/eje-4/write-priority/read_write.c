#include "read_write.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct __MyStruct {
    int reading; //Controla cantidad en simultaneo (leyendo o esperando)
		int writing; //Cantidad de escritores (escribiendo o esperando)
    pthread_mutex_t mutex; // Mutex para manejo de prioridades
		pthread_cond_t cv;
    sem_t sem; // Mutex para writing
};

struct_t rw_struct_init(){
	struct_t re_struct = malloc(sizeof(struct __MyStruct));
	re_struct->reading = 0;
	re_struct->writing = 0;
	pthread_mutex_init(&re_struct->mutex, NULL);
	pthread_cond_init(&re_struct->cv, NULL);
	sem_init(&re_struct->sem, 0,1);
	return re_struct;
}

void write_lock(struct_t rw_struct){
	sem_wait(&rw_struct->sem);
	rw_struct->writing ++; // Escritores en espera
	sem_post(&rw_struct->sem);
	pthread_mutex_lock(&rw_struct->mutex);
	sem_wait(&rw_struct->sem);
	while (rw_struct->reading){ // Pasa si no hay lectores
		sem_post(&rw_struct->sem);
		pthread_cond_wait(&rw_struct->cv, &rw_struct->mutex);
		sem_wait(&rw_struct->sem);
	}
	sem_post(&rw_struct->sem);
}

void write_unlock(struct_t rw_struct){
	rw_struct->writing--;
	pthread_cond_broadcast(&rw_struct->cv); //levanto lectores / escritores
	pthread_mutex_unlock(&rw_struct->mutex);
}

void read_lock(struct_t rw_struct){
	pthread_mutex_lock(&rw_struct->mutex);
	sem_wait(&rw_struct->sem);
	while (rw_struct->writing){ // si hay escritores espero
		sem_post(&rw_struct->sem);
		pthread_cond_signal(&rw_struct->cv); ///
		pthread_cond_wait(&rw_struct->cv, &rw_struct->mutex);
		sem_wait(&rw_struct->sem);
	}
	rw_struct->reading++;
	sem_post(&rw_struct->sem);
	pthread_cond_broadcast(&rw_struct->cv);
	pthread_mutex_unlock(&rw_struct->mutex); // libero para otros lectores
}

void read_unlock(struct_t rw_struct){
	sem_wait(&rw_struct->sem);
	rw_struct->reading--;
	sem_post(&rw_struct->sem);
}

void rw_struct_close(struct_t rw_struct){
	pthread_cond_destroy(&rw_struct->cv);
	pthread_mutex_destroy(&rw_struct->mutex);
	sem_destroy(&rw_struct->sem);
}
