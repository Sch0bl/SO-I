#include "read_write.h"
#include <pthread.h>
#include <stdio.h>

void rw_struct_init(struct_t *rw_struct){
	pthread_mutex_init(&rw_struct->mutex, NULL);
	pthread_cond_init(&rw_struct->cv, NULL);
	sem_init(&rw_struct->sem, 0,1);
}

void write_lock(struct_t *rw_struct){
	sem_wait(&rw_struct->sem);
	rw_struct->writing ++;
	sem_post(&rw_struct->sem);
	pthread_mutex_lock(&rw_struct->mutex);
	sem_wait(&rw_struct->sem);
	while (rw_struct->reading){
		sem_post(&rw_struct->sem);
		pthread_cond_wait(&rw_struct->cv, &rw_struct->mutex);
		sem_wait(&rw_struct->sem);
	}
	sem_post(&rw_struct->sem);
}

void write_unlock(struct_t *rw_struct){
	rw_struct->writing--;
	pthread_cond_signal(&rw_struct->cv);
	pthread_mutex_unlock(&rw_struct->mutex);
}

void read_lock(struct_t *rw_struct){
	pthread_mutex_lock(&rw_struct->mutex);
	sem_wait(&rw_struct->sem);
	while (rw_struct->writing){
		sem_post(&rw_struct->sem);
		pthread_cond_signal(&rw_struct->cv); ////
		pthread_cond_wait(&rw_struct->cv, &rw_struct->mutex);
		sem_wait(&rw_struct->sem);
	}
	rw_struct->reading++;
	sem_post(&rw_struct->sem);
	pthread_cond_signal(&rw_struct->cv);
	pthread_mutex_unlock(&rw_struct->mutex);
}

void read_unlock(struct_t *rw_struct){
	sem_wait(&rw_struct->sem);
	rw_struct->reading--;
	sem_post(&rw_struct->sem);
}

void rw_struct_close(struct_t *rw_struct){
	pthread_cond_destroy(&rw_struct->cv);
	pthread_mutex_destroy(&rw_struct->mutex);
	sem_destroy(&rw_struct->sem);
}
