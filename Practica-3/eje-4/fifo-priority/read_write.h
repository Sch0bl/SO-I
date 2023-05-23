#ifndef __READ__WRITE__
#define __READ__WRITE__
#include <pthread.h>
#include <semaphore.h>

struct __MyStruct {
    int reading;
		int writing;
    pthread_mutex_t mutex;
		pthread_cond_t cv;
    sem_t sem;
};

typedef struct __MyStruct struct_t;

/************rw_struct_init**************/
/* Inicializa la estructura de datos para el */
/* manejo de mutex, condiciones variables y */ 
/* semaforos. */
void rw_struct_init(struct_t* rw_strcut);

void read_lock(struct_t* rw_struct);

void read_unlock(struct_t* rw_struct);

void write_lock(struct_t* rw_struct);

void write_unlock(struct_t* rw_struct);

void rw_struct_close(struct_t *rw_struct);

#endif

