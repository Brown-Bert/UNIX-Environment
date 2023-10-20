#include <bits/types/sigset_t.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <csignal>
static void h(int s) { write(1, "!", 1); }
int main() {
  sigset_t set;
  sigemptyset(&set);
  int i;
  sigaddset(&set, SIGINT);
  signal(SIGINT, h);
  while (1) {
    sigprocmask(SIG_BLOCK, &set, NULL);
    for (i = 0; i < 5; i++) {
      write(1, "*", 1);
      sleep(1);
    }
    sigprocmask(SIG_UNBLOCK, &set, NULL);
    write(1, "\n", 1);
  }
  exit(0);
}
