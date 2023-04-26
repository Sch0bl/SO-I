#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_VISITANTES 1000000

int visitantes = 0;

void* proceso(void* arg){
	int i;
	int id = arg - (void*)0;
	for(int i = 0; i < N_VISITANTES; ++i){
		int c;
		//Duerme t1 para darle ventaja a t0
		if(id == 1 && i == 0) 
			sleep(1);
		//Duerme t1 con el valor N - 1, epera a que t0 actualize su valor a 1.
		if(id == 1 && i == N_VISITANTES - 1)
			sleep(2);
		c = visitantes;
		//Esperan que los otros procesos hagan sus operaciones para depues sobreescribirlas
		if((id == 0 && i == 0) || (id == 1 && i == N_VISITANTES - 1))
			sleep(2);
		visitantes = c + 1;
		//Le da tiempo a t1 para soobreescribir el valor N - 1
		if(id == 0 && i == 0)
			sleep(2);
	}
	return NULL;
}

int main(){
	pthread_t m0, m1;

	pthread_create(&m0, NULL, proceso, 0);
	pthread_create(&m1, NULL, proceso, (void*)1);
	
	// hacer join
	pthread_join(m0,NULL);
	pthread_join(m1,NULL);

	printf("El numero de visitantes es : %d\n", visitantes);
	
	return 0;
}
