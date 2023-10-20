#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
static void h(int s){
	write(1, "!", 1);
}
int main(){
	int i;
	signal(SIGINT, h);
	for(i = 0; i < 10; i++) {
		write(1, "*", 1);
		sleep(1);
	}
	write(1, "\n", 1);
	
	exit(0);
}
