#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <csignal>
#include <iostream>
#include <iterator>
using namespace std;
static void h(int s) { cout << "!"; }
int main() {
  signal(SIGINT, h);
  int64_t count = 0;
  while (1) {
    for (int i = 0; i < 10; i++) {
      count = 0;
      cout << "*";
      fflush(stdout);
      while (count < 1000000000) {
        count++;
      }
      //   sleep(1);
    }
    cout << endl;
  }
  //   while (count < 10000000000) {
  //     count++;
  //   }
  exit(0);
}