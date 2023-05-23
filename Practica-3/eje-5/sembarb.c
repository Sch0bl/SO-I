#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define THREADS 10 
#define SILLAS	5 

int id;                 //Variable que identifica a los clientes
int pos;                // Posición en la queue (sillas) de los clientes
int sig;                // Cliente que sigue en la queue
int clientes;           // Cantidad de clientes esperando
int turno[SILLAS];      // Queue (sillas)
									  
sem_t sem_id;           // Mutex para manejo de id de clientes en maneja_proceso

sem_t sem_mutex;        // Mutex
sem_t sem_clientes;     // Manda señal al barbero para despertarse
sem_t sem_barbero;      // manda señal para asignar el turno
sem_t sem_asignado;     // manda señal al barbero de que el turno fue asignado
sem_t sem_cortando;     // concurrencia
sem_t sem_pagando;      // concurrencia

void cortando(int i){
    printf("Le estoy cortando al cliente %d\n", i);
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
    printf("Me pagan\n");
    sleep(1);
    return;
}

void* barbero_func(){
    while(1){
				//Espera a que entre un cliente para despertarse
        sem_wait(&sem_clientes);

				//Manda señal a los clientes que está listo para cortar
				sem_post(&sem_barbero);

				//Espera la asignación del turno
				sem_wait(&sem_asignado);

        sem_post(&sem_cortando);
				//Empieza a cortar
        cortando(turno[sig]);

				//Espera a que le paguen
        sem_wait(&sem_pagando);
        me_pagan();

				//Da el turno
				sem_wait(&sem_mutex);
        sig++;
				sig = sig % SILLAS;
				sem_post(&sem_mutex);
    }
}

void* clientes_func(void* arg){
    int num = arg - NULL; //id
													
		sem_wait(&sem_mutex);    // Lock
    if (clientes == SILLAS){ // Todas las sillas están ocupadas
        printf("Barberia llena.\n");
				sem_post(&sem_mutex);
        return NULL;
    }
    clientes++;
    printf("Entra cliente: %d. Se sienta en silla: %d\n", num, pos);
		//Guarda el turno del clinte en la queue
    turno[pos] = num;
    pos = (pos + 1) % SILLAS;
		sem_post(&sem_mutex);        // Unlock

		sem_post(&sem_clientes);     //Cliente esperando
		sem_wait(&sem_barbero);      //Epera a que el barbero los llame
																 
    while(turno[sig] != num){    // Asignación del turno
			sem_post(&sem_barbero);
			sleep(1);
			sem_wait(&sem_barbero);
    }

		sem_wait(&sem_mutex);        // Lock
    clientes--;                  // El cliente deja libre la silla
		sem_post(&sem_mutex);        // Unlock
																 
		sem_post(&sem_asignado);     // Turno asignado

    sem_wait(&sem_cortando);     // Espera a que el barbero empiece con el corte
    me_cortan(num);
    sem_post(&sem_pagando);      // Cliente abona el corte
    pagando(num);
    printf("Corte listo. Tirando facha.\n");
    return NULL;
}


void* maneja_proceso(void* arg){
    while(1){
				int local_id;
        sleep(1);
				sem_wait(&sem_id);
        id = (id + 1) % 10000;
				local_id = id;
				sem_post(&sem_id);
        clientes_func(local_id + (void*) 0); 
    }
}

int main(){
    pthread_t barbero, thread[THREADS];
		sem_init(&sem_id, 0, 1);
		sem_init(&sem_mutex, 0, 1);
    sem_init(&sem_asignado, 0, 0);
    sem_init(&sem_clientes, 0, 0);
    sem_init(&sem_cortando, 0, 0);
    sem_init(&sem_pagando,0,0);
    sem_init(&sem_barbero,0,0);
    int i;
    pthread_create(&barbero, NULL, barbero_func, NULL);
    for (i = 0; i < THREADS; i++)
        pthread_create(&thread[i], NULL, maneja_proceso,NULL);
    
    pthread_join(barbero, NULL); 
}
