#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
static volatile int loop = 1;
static void h(int s){
	loop = 0;

}

int main(){
	int64_t count = 0;
	signal(SIGALRM, h);
	alarm(5);
	while(loop)count++;	
	printf("%ld\n", count);
	exit(0);
}
