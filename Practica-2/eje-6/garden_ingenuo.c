#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_VISITANTES 1000000

int visitantes = 0;

void* molinete0(void* arg){
	for(int i = 0; i < N_VISITANTES; ++i){
		visitantes++;
	}
	return NULL;
}

int main(){
	pthread_t m0, m1;

	pthread_create(&m0, NULL, molinete0, NULL);
	pthread_create(&m1, NULL, molinete0, NULL);
	
	// hacer join
	pthread_join(m0,NULL);
	pthread_join(m1,NULL);

	printf("El numero de visitantes es : %d\n", visitantes);
	
	return 0;
}
