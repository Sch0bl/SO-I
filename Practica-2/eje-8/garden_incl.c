#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_VISITANTES 10000000

int visitantes = 0;

static inline void incl(int *p) {
	asm("incl %0" : "+m"(*p) : : "memory");
}

void* proceso(void* arg){
	for(int i = 0; i < N_VISITANTES; ++i){
		incl(&visitantes);
	}
	return NULL;
}

int main(){
	pthread_t m0, m1;

	pthread_create(&m0, NULL, proceso, NULL);
	pthread_create(&m1, NULL, proceso, NULL);
	
	// hacer join
	pthread_join(m0,NULL);
	pthread_join(m1,NULL);

	printf("El numero de visitantes es : %d\n", visitantes);
	
	return 0;
}
