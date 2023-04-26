#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_VISITANTES 1000000

int visitantes;

int flag[2] = {0, 0}; // si flag = 1 entonces quiero entrar
											// a la región crítica

int turno = 0; //0: turno thread 0 y 1: turno thread 1

//lock -> permite generar cositas

void* molinete0(void* arg){
	for(int i = 0; i < N_VISITANTES; ++i){
		flag[0] = 1;
		turno = 1;
		asm("mfence"); // Sincronización de Procesadores
		while(flag[1] == 1 && turno == 1){
	//		sleep(1);
		}
		//Reclamo la región crítica
		visitantes++;
		//libero la región
		//unlock(){
		flag[0] = 0;
		//}
	}
	return NULL;
}

void* molinete1(void* arg){
	for(int i = 0; i < N_VISITANTES; ++i){
		flag[1] = 1;
		turno = 0;
		asm("mfence"); // Sincronización de Procesadores
		while(flag[0] == 1 && turno == 0){
		//	sleep(1);
		}
		//Reclamo la región crítica
		visitantes++;
		//libero :: región
		//unlock(){
		flag[1] = 0;
		//}
	}
	return NULL;
}

int main(){
	pthread_t m0, m1;

	pthread_create(&m0, NULL, molinete0, NULL);
	pthread_create(&m1, NULL, molinete1, NULL);
	
	// hacer join
	pthread_join(m0,NULL);
	pthread_join(m1,NULL);

	printf("El numero de visitantes es : %d\n", visitantes);
	
	return 0;
}
