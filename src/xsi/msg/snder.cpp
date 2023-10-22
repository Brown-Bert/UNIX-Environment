#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <cstdlib>
#include <cstring>

#include "proto.h"

int main(int argc, char* argv[]) {
  key_t key;
  int msgid;
  struct data sndbuf;

  key = ftok(PATHNAME, PROJ);
  if (key < 0) {
    perror("ftok()");
    exit(1);
  }
  msgid = msgget(key, 0);
  if (msgid < 0) {
    perror("msgget()");
    exit(1);
  }
  sndbuf.mtype = 1;
  strcpy(sndbuf.name, "yzx");
  sndbuf.math = 100;
  sndbuf.chinese = 100;
  int res = msgsnd(msgid, &sndbuf, sizeof(sndbuf), 0);
  if (res < 0) {
    perror("msgsnd()");
    exit(1);
  }
  puts("OK");
  exit(0);
}