#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstddef>
#include <cstdlib>
#define LEFT 30000000
#define RIGHT 30000200
#define THNUM (RIGHT - LEFT + 1)

struct node {
  int n;
};

static void* prime(void* p) {
  //   puts("123");
  int num = ((struct node*)p)->n;
  // free(p);
  int flag = 1;
  for (int i = 2; i <= num / 2; i++) {
    if (num % i == 0) {
      flag = 0;
      break;
    }
  }
  if (flag) printf("%d is primer\n", num);

  pthread_exit(p);
}

int main() {
  pthread_t tid[THNUM];
  struct node* p;
  void* res = NULL;

  for (int i = LEFT; i <= RIGHT; i++) {
    p = (struct node*)malloc(sizeof(*p));
    if (p == NULL) {
      perror("malloc()");
      exit(1);
    }
    p->n = i;
    int err = pthread_create(&tid[i - LEFT], NULL, prime, p);
    if (err) {
      fprintf(stderr, "pthread_creat failed: %s\n", strerror(err));
      exit(1);
    }
    // free(p);
  }
  for (int i = LEFT; i <= RIGHT; i++) {
    pthread_join(tid[i - LEFT], &res);
    free(res);
  }
  exit(0);
}