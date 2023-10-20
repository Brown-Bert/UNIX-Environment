#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	int64_t count = 0;
	time_t end = time(NULL) + 5;

	while(time(NULL) <= end) count++;
	printf("%ld\n", count);
	exit(0);
}
