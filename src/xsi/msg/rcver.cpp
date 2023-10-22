#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <cstddef>

#include "proto.h"

int main(int argc, char* argv[]) {
  key_t key;
  int msgid;
  struct data rcvbuf;
  ssize_t size;

  key = ftok(PATHNAME, PROJ);
  if (key < 0) {
    perror("ftok()");
    exit(1);
  }
  msgid = msgget(key, IPC_CREAT | 0600);
  if (msgid < 0) {
    perror("msgget()");
    exit(1);
  }
  while (1) {
    puts("123");
    size = msgrcv(msgid, &rcvbuf, sizeof(rcvbuf), 0, 0);
    puts("456");
    if (size < 0) {
      perror("msgrcv()");
      exit(1);
    }
    printf("mtype = %ld\n", rcvbuf.mtype);
    printf("name = %s\n", rcvbuf.name);
    printf("math = %d\n", rcvbuf.math);
    printf("chinese = %d\n", rcvbuf.chinese);
  }
  int res = msgctl(msgid, IPC_RMID, NULL);
  if (res < 0) {
    perror("msgctl()");
    exit(1);
  }
  exit(0);
}