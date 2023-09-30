#include <bits/types/sigset_t.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <csignal>
#include <cstddef>
static void h(int s) { write(1, "!", 1); }
int main() {
  sigset_t set, oset;
  sigemptyset(&set);
  int i;
  sigaddset(&set, SIGINT);
  signal(SIGINT, h);
  sigprocmask(SIG_BLOCK, &set, &oset);
  while (1) {
    for (i = 0; i < 5; i++) {
      write(1, "*", 1);
      sleep(1);
    }
    write(1, "\n", 1);
    sigsuspend(&oset);
    // sigset_t tempset;
    // sigprocmask(SIG_SETMASK, &oset, &tempset);
    // pause();
    // sigprocmask(SIG_SETMASK, &tempset, NULL);
  }
  exit(0);
}
