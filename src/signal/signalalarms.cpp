#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void h(int s) { printf("123"); }

int main() {
  int64_t count = 0;
  signal(SIGALRM, h);
  unsigned int res = alarm(100);
  unsigned int res1 = alarm(5);
  unsigned int res2 = alarm(10);
  printf("res = %d\n", res);
  printf("res1 = %d\n", res1);
  printf("res2 = %d\n", res2);
  exit(0);
}
