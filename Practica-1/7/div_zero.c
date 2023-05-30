#include <stdio.h>
#include <signal.h>

int denom = 0;
void handler(int s) {
	printf("ouch!\n");
	denom = 1;
}

int main() {
	signal(SIGFPE, handler);
	int r = (int) 1 / (int)denom;
  printf("%d\n", denom);
	printf("r = %d\n", r);
	return 0;
}
