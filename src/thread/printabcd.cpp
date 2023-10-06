#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <queue>
using namespace std;
#define LEFT 30000000
#define RIGHT 30000200
#define THNUM 4
pthread_mutex_t mut;
static int flag =
    1;  // 给下游线程拿去做判断，如果是-1就表明任务全部做完了，需要退出程序
static int tasksize = 0;  // 表明当前的任务队列中有多少任务
#define MAXSIZE 1024  // 表明任务队列最大是1024，只能往任务队列里面放1024个任务
queue<int> tasks;  // 任务队列
/**
    只用四个线程去计算201个数是不是素数
*/
static void *func(void *) {
  while (1) {
    if ((flag == -1) && (tasks.empty())) {
      pthread_exit(NULL);
    } else {
      if (tasksize != 0) {
        pthread_mutex_lock(&mut);
        int num = tasks.front();
        tasks.pop();
        tasksize--;
        pthread_mutex_unlock(&mut);
        int f = 1;
        for (int i = 2; i <= num / 2; i++) {
          if (num % i == 0) {
            f = 0;
            break;
          }
        }
        if (f) printf("%d is primer\n", num);
      }
    }
  }
}
static void *function(void *) {
  int i = LEFT;
  while (i <= RIGHT) {
    if (tasksize < MAXSIZE) {
      //   sched_yield();
      //   sleep(1);
      pthread_mutex_lock(&mut);
      // puts("123");
      tasksize++;
      tasks.push(i);
      pthread_mutex_unlock(&mut);
      i++;
    }
  }
  pthread_mutex_lock(&mut);
  flag = -1;
  pthread_mutex_unlock(&mut);
  pthread_exit(NULL);
}

int main() {
  pthread_t tid[THNUM];
  pthread_t id;
  int res;
  res = pthread_create(&id, NULL, function, NULL);
  if (res) {
    fprintf(stderr, "pthread_create failed: %s", strerror(res));
    exit(1);
  }
  for (int i = 0; i < THNUM; i++) {
    res = pthread_create(tid + i, NULL, func, NULL);
    if (res) {
      fprintf(stderr, "pthread_create failed: %s", strerror(res));
      exit(1);
    }
  }
  for (int i = 0; i < THNUM; i++) {
    pthread_join(tid[i], NULL);
  }
  pthread_join(id, NULL);
  pthread_mutex_destroy(&mut);
  exit(0);
}