#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_MOLINETES 10
#define N_VISITANTES 1000000

int visitantes = 0;

int flag[N_MOLINETES]; //ya están inicializado en 0 :)


int turno[N_MOLINETES]; //0: turno thread 0 y 1: turno thread 1

//lock -> permite generar cositas

int max(int* turnos_pointer){return 0;}

void lamport_lock(int persona) {
	/* Calcula el npumero de turno */
	flag[persona] = 1;
	turno[persona] = 1 + max(turno);
	flag[persona] = 0;

	for(int j = 0; j < N_MOLINETES; j++){
		/* Si el hilo j está calculando su número, espera a que termine */
		while(flag[j]){/*bussy waiting*/}
		
		/* Si el hilo j tiene más prioridad, espera q que ponga su número a cero */
		/* j tiene más prioridad si su número de turno es más bajo que el de i */
		/* o bien si es el mismo número y además j es menor que i */

		while((turno[j] != 0) &&
				  ((turno[j] < turno[persona])) ||
					((turno[j] == turno[persona] &&
					(j < persona))))
	}	
}

void* molinete(void* arg){
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

	pthread_create(&m0, NULL, molinete, NULL);
	pthread_create(&m1, NULL, molinete, NULL);
	
	// hacer join
	pthread_join(m0,NULL);
	pthread_join(m1,NULL);

	printf("El numero de visitantes es : %d\n", visitantes);
	
	return 0;
}
