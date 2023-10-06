#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <queue>
#include <string>
using namespace std;

#define _threadNum 3          // 表示开几个写线程
#define _blockSize 64 * 1024  // 表示一个块的大小 以字节为单位
int _taskSize = 3;     // 目前任务队列中实际上有多少个任务
pthread_mutex_t _mut;  // 文件上锁
string _souFileName = "./prime_thread.cpp";      // 源文件位置
string _desFileName = "./prime_threadcopy.cpp";  // 目的文件位置
off_t _offset = 0;  // 写文件的时候需要同步文件偏移量
int count = 0;  // 统计一轮中有多少个线程已经完成写入的操作
typedef struct node {
  char *_buf;     // 存放数据的buf
  int _realsize;  // 数据的实际大小
  int _index;     // 数据块的顺序
} node;
queue<node> _tasks;  // 任务队列
bool _fileFlag = false;  // false表示这个文件还没有读完，true表示这个文件读完了

pthread_cond_t
    _condStoD;  // 源文件读取线程往目的文件写入线程发通知所需的条件变量
pthread_cond_t
    _condDtoS;  // 目的文件写入线程往源文件读取线程发送通知所需的条件变量
pthread_spinlock_t
    sp;  // 设置一个自旋锁用于通知多个线程在一轮操作中已经全部完成

/**
    初始化锁
  */
void initMutexAndCondition() {
  pthread_mutex_init(&(_mut), NULL);
  pthread_cond_init(&(_condStoD), NULL);
  pthread_cond_init(&(_condDtoS), NULL);
  pthread_spin_init(&(sp), 0);
}

/**
  销毁锁
*/
void destroyMutexAndCondition() {
  pthread_mutex_destroy(&(_mut));
  pthread_cond_destroy(&(_condStoD));
  pthread_cond_destroy(&(_condDtoS));
  pthread_spin_destroy(&(sp));
}

/**
    读取一块任务
    @parameter fd: 文件描述符
    @return 读取成功与否
  */
bool readOneBlock(int fd, node *oneNode) {
  oneNode->_buf = (char *)malloc(_blockSize);
  if (oneNode->_buf == NULL) {
    return false;
  }
  int realSize;
  realSize = read(fd, oneNode->_buf, _blockSize);
  if (realSize == -1) {
    perror("read()");
    return false;
  }
  oneNode->_realsize = realSize;
  return true;
}

/**
    开启线程，打开文件往任务队列中按照数据块的大小往里面写
  */
void *(openSouFileAndRead)(void *p) {
  int fd;
  fd = open(_souFileName.c_str(), O_RDONLY);
  if (fd < 0) {
    perror("open()");
    // 文件打开异常，直接退出进程
    exit(1);
  }
  struct timespec exceedTime;
  exceedTime.tv_nsec = 0;
  exceedTime.tv_sec = 1;
  while (1) {
    pthread_cond_timedwait(&(_condDtoS), &(_mut), &exceedTime);
    if (_fileFlag) {
      // 文件读完了
      break;
    }
    if (_taskSize == 0) {
      // 说明任务队列中已经没有任务了，需要往任务队列中放任务
      for (int i = 0; i < _threadNum; i++) {
        int flag = 0;
        node oneNode;
        oneNode._index = i;
        bool res;
        while (1) {
          res = readOneBlock(fd, &oneNode);
          if (res) {
            break;
          }
        }
        if (oneNode._realsize == 0) {
          // 文件读完了
          _fileFlag = true;
          break;
        } else {
          _tasks.push(oneNode);
          _taskSize++;
        }
      }
      pthread_cond_broadcast(&(_condStoD));
      pthread_mutex_unlock(&(_mut));
    }
  }
  close(fd);
  pthread_exit(NULL);
}

/**
    开启写线程， 从任务队列中拿数据写入目的文件
  */
void *openDesFileAndWrite(void *p) {
  int fd;
  fd = open(_desFileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
  if (fd < 0) {
    perror("open()");
    // 文件打开异常，直接退出进程
    exit(1);
  }
  struct timespec exceedTime;
  exceedTime.tv_nsec = 0;
  exceedTime.tv_sec = 1;
  pthread_cond_wait(&(_condStoD), &(_mut));
  while (1) {
    if (_taskSize == 0) {
      if (_fileFlag) break;
      pthread_cond_signal(&(_condDtoS));
      pthread_mutex_unlock(&(_mut));
      while (1) {
        pthread_spin_lock(&(sp));
        if (count == _threadNum) {
          count = 0;
          break;
        }
      }
      pthread_spin_unlock(&(sp));
      _offset = lseek(fd, 0, SEEK_END);
    } else {
      node oneNode = _tasks.front();
      _tasks.pop();
      _taskSize--;
      pthread_mutex_unlock(&(_mut));
      lseek(fd, _offset + oneNode._index * _blockSize, SEEK_SET);
      write(fd, oneNode._buf, oneNode._realsize);
      pthread_spin_lock(&(sp));
      count++;
      pthread_spin_unlock(&(sp));
    }
  }
  pthread_mutex_unlock(&(_mut));
  close(fd);
  pthread_exit(NULL);
}

void copyFile() {
  // 开启线程，打开文件往任务队列中按照数据块的大小往里面写
  pthread_t souTid;
  int res;
  res = pthread_create(&souTid, NULL, openSouFileAndRead, NULL);
  if (res) {
    fprintf(stderr, "pthread_create: %s", strerror(res));
    exit(1);
  }
  pthread_cond_signal(&_condDtoS);
  // 开启写线程，从任务队列里面拿取任务写入目标文件
  pthread_t desTid[_threadNum];
  for (int i = 0; i < _threadNum; i++) {
    res = pthread_create(&desTid[i], NULL, openDesFileAndWrite, NULL);
    if (res) {
      fprintf(stderr, "pthread_create: %s", strerror(res));
      exit(1);
    }
  }
  // 收尸
  pthread_join(souTid, NULL);
  for (int i = 0; i < _threadNum; i++) {
    pthread_join(desTid[i], NULL);
  }
}

// int main() {
//   initMutexAndCondition();
//   copyFile();
//   destroyMutexAndCondition();
//   return 0;
// }