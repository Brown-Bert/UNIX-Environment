#ifndef COPYFILE_H_
#define COPYFILE_H_
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <string>
using namespace std;

// struct node;
class CopyFileClass {
 private:
  int _threadNum;       // 表示开几个写线程
  int _blockSize;       // 表示一个块的大小 以字节为单位
  string _souFileName;  // 源文件位置
  string _desFileName;  // 目的文件位置

 public:
  CopyFileClass(int _threadNum, int _blockSize);
  /**
    初始化源文件路径和目的文件路径
  */
  void initFilePath(string _souFileName, string _desFileName);

  /**
    初始化锁
  */
  void initMutexAndCondition();

  /**
    销毁锁
  */
  void destroyMutexAndCondition();

  /**
    copy文件
  */
  void copyFile();
};

#endif