#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define CLIENTES 10
#define SILLAS 5

int id; //Variable que identifica a los clientes
int pos; // Posición en la queue (sillas) de los clientes
int sig; // Cliente que sigue en la queue
int cantidad_de_gente; // Cantidad de clientes esperando
int turno[SILLAS]; // Queue (sillas)

pthread_cond_t cv;
pthread_mutex_t mutex;
pthread_mutex_t mutex_id;
/* pthread_mutex_t mutex_sig; */
sem_t sem_clientes;
sem_t sem_cortando;
sem_t sem_pagando;

void cortando(){
    printf("Estoy cortando\n");
    sleep(1);
    return;
}

void me_cortan(int i){
    printf("Me estan cortando, cliente %d\n", i);
    sleep(1);
    return;
}

void pagando(int i){
    printf("Estoy pagando, cliente %d\n", i);
    sleep(1);
    return;
}

void me_pagan(){
    printf("Me estan pagando\n");
    sleep(1);
    return;
}

void* barbero_func(){
    while(1){
				//Espera a que entre un cliente para despertarse
        sem_wait(&sem_clientes);
        sem_post(&sem_cortando);
        cortando();
        sem_wait(&sem_pagando);
        me_pagan();
        pthread_mutex_lock(&mutex);
        sig++;
		    sig = sig % SILLAS;
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cv);
    }
}

void* clientes_func(void* arg){
    pthread_mutex_lock(&mutex);
    int num = arg - NULL;
    if (cantidad_de_gente == SILLAS){
        printf("Me voy\n");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    printf("Entra cliente: %d\n", num);
    turno[pos] = num;
    pos = (pos + 1) % SILLAS;
    cantidad_de_gente++;
    printf("Cantidad de gente: %d\n", cantidad_de_gente);
		sem_post(&sem_clientes);
    while(turno[sig] != num){
        pthread_cond_wait(&cv, &mutex);
				printf("Sig = %d, Turno de %d\n",sig,  turno[sig]);
    }
    cantidad_de_gente--;
    pthread_mutex_unlock(&mutex);
    sem_wait(&sem_cortando);
    me_cortan(num);
    sem_post(&sem_pagando);
    pagando(num);
    printf("Me cortaron y pagué\n");
    return NULL;
}


void* maneja_proceso(void* arg){
    while(1){
				int local_id;
        sleep(1);
        pthread_mutex_lock(&mutex_id);
        id = (id + 1) % 10000;
				local_id = id;
        pthread_mutex_unlock(&mutex_id);
        clientes_func(local_id + (void*) 0); 
    }
}

int main(){
    pthread_t barbero, thread[CLIENTES];

    
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_id, NULL);
    /* pthread_mutex_init(&mutex_sig, NULL); */
    pthread_cond_init(&cv, NULL);
    sem_init(&sem_clientes, 0, 0);
    sem_init(&sem_cortando, 0, 0);
    sem_init(&sem_pagando,0,0);
    int i;
    pthread_create(&barbero, NULL, barbero_func, NULL);
    for (i = 0; i < CLIENTES; i++)
        pthread_create(&thread[i], NULL, maneja_proceso,NULL);
    
    pthread_join(barbero, NULL); 
}
