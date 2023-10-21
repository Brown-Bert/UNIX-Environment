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

// int main() {
//   int fd =
//       open("/home/yzx/vscode/workspace/unixenv/src/mman/test.txt", O_WRONLY);
//   if (fd < 0) {
//     perror("open()");
//     exit(1);
//   }
//   std::cout << fd << std::endl;
//   pthread_t tid;
//   int res = pthread_create(&tid, NULL, func, NULL);
//   if (res < 0) {
//     fprintf(stderr, "pthread_create()");
//     close(fd);
//     exit(1);
//   }
//   pthread_join(tid, NULL);
//   write(fd, "123456789", 9);
//   close(fd);
//   exit(0);
// }

/**
    结论是父子线程共享文件描述符表，子线程关闭文件，会导致父线程使用不了文件
*/

/**
  测试fileno和fdopen
*/
int main() {
  /**
    测试fdopen
  */
  // char buf[6] = "hello";
  // int fd =
  //     open("/home/yzx/vscode/workspace/unixenv/src/mman/test.txt", O_WRONLY);
  // printf("%d\n", fd);
  // FILE* fp = fdopen(fd, "w");
  // int res = fwrite(buf, 5, 1, fp);
  // printf("%d\n", res);
  // // fclose(fp);
  // FILE* ft = fdopen(fd, "w");
  // if (ft == NULL) {
  //   puts("shibai");
  // }
  // int f =
  //     open("/home/yzx/vscode/workspace/unixenv/src/mman/test1.txt",
  //     O_WRONLY);
  // printf("%d\n", f);

  /**
    测试fileno
  */
  int f1 =
      open("/home/yzx/vscode/workspace/unixenv/src/mman/test.txt", O_WRONLY);
  // int fd = fileno(fp);
  // printf("%d\n", fd);
  // fclose(fp);
  // close(fd);
  sleep(100);
  int f2 =
      open("/home/yzx/vscode/workspace/unixenv/src/mman/test.txt", O_WRONLY);

  // printf("%d\n", f);
  exit(0);
}
/**
  使用fdopen的时候，会增加文件打开表的引用次数
  使用fileno的时候，不会增加文件打开表的额引用次数
  关闭一个文件的流程（猜测）：
  使用close关闭，参数是文件描述符
*/