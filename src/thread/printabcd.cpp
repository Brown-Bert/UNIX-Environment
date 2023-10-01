#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cstdlib>

#define THNUM 4
pthread_mutex_t mut[THNUM];
struct node {
  int n;
};
static void *func(void *p) {
  int i = ((struct node *)p)->n;
  int c = 'a' + i;
  //   puts("123");
  while (1) {
    pthread_mutex_lock(mut + i);
    printf("%c", c);
    pthread_mutex_unlock(mut + ((i + 1) % 4));
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t tid[THNUM];
  int res;
  struct node *p, q;
  for (int i = 0; i < THNUM; i++) {
    pthread_mutex_init(mut + i, NULL);
    pthread_mutex_lock(mut + i);
    p = (struct node *)malloc(sizeof(*p));
    if (p == NULL) {
      perror("malloc()");
      exit(1);
    }
    p->n = i;
    res = pthread_create(tid + i, NULL, func, p);
    if (res) {
      fprintf(stderr, "pthread_create failed: %s", strerror(res));
      exit(1);
    }
  }
  pthread_mutex_unlock(mut);
  alarm(2);
  for (int i = 0; i < THNUM; i++) {
    pthread_join(tid[i], NULL);
  }
  exit(0);
}