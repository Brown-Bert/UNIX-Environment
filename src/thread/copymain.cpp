#include "copyfile.h"
int main() {
  CopyFileClass copyFile(3, 64 * 1024);
  copyFile.initFilePath(
      "/home/yzx/vscode/workspace/unixenv/src/thread/test.txt",
      "/home/yzx/vscode/workspace/unixenv/src/thread/testcopy.txt");
  copyFile.initMutexAndCondition();
  copyFile.copyFile();
  copyFile.destroyMutexAndCondition();
  return 0;
}