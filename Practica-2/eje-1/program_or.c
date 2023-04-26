#include <stdio.h>
#include <stdlib.h>
#define LEN_ARRAY 12

int array[LEN_ARRAY] = { 1 , 2 , 3 , 2 , 2, 3, 2, 1, 2, 3, 2, 1} ;

int encontrado = 0;

int main(){

	for(int i = 0; i < LEN_ARRAY; i++){
		encontrado = encontrado || (array[i] == 42);
	}

	return 0;
}


