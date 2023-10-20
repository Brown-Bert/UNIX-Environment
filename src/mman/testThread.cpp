/**
    用于测试父线程打开一个文件，子线程去关闭，之后父线程的文件是不是也被关闭了（猜测应该会）
*/
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cstddef>
#include <iostream>

void* func(void*) {
  //   close(3);
  pthread_exit(NULL);
}

int main() {
  int fd =
      open("/home/yzx/vscode/workspace/unixenv/src/mman/test.txt", O_WRONLY);
  if (fd < 0) {
    perror("open()");
    exit(1);
  }
  std::cout << fd << std::endl;
  pthread_t tid;
  int res = pthread_create(&tid, NULL, func, NULL);
  if (res < 0) {
    fprintf(stderr, "pthread_create()");
    close(fd);
    exit(1);
  }
  pthread_join(tid, NULL);
  write(fd, "123456789", 9);
  close(fd);
  exit(0);
}

/**
    结论是父子线程共享文件描述符表，子线程关闭文件，会导致父线程使用不了文件
*/