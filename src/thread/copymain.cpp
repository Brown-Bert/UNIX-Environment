#include <openssl/evp.h>
#include <openssl/md5.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "copyfile.h"

std::string calculateMD5(const std::string& input) {
  EVP_MD_CTX* md5Context = EVP_MD_CTX_new();
  if (md5Context == nullptr) {
    std::cerr << "无法创建 MD5 上下文。" << std::endl;
    return "";
  }

  if (EVP_DigestInit_ex(md5Context, EVP_md5(), nullptr) != 1) {
    std::cerr << "无法初始化 MD5 上下文。" << std::endl;
    EVP_MD_CTX_free(md5Context);
    return "";
  }

  if (EVP_DigestUpdate(md5Context, input.c_str(), input.length()) != 1) {
    std::cerr << "无法更新 MD5 上下文。" << std::endl;
    EVP_MD_CTX_free(md5Context);
    return "";
  }

  unsigned char digest[MD5_DIGEST_LENGTH];
  if (EVP_DigestFinal_ex(md5Context, digest, nullptr) != 1) {
    std::cerr << "无法获取 MD5 哈希值。" << std::endl;
    EVP_MD_CTX_free(md5Context);
    return "";
  }

  EVP_MD_CTX_free(md5Context);

  std::stringstream ss;
  for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    ss << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(digest[i]);
  }

  return ss.str();
}

int main() {
  std::string sourceFileName =
      "/home/yzx/vscode/workspace/unixenv/src/thread/test.txt";
  std::string destination =
      "/home/yzx/vscode/workspace/unixenv/src/thread/testcopy.txt";
  CopyFileClass copyFile(3, 64 * 1024);
  copyFile.initFilePath(sourceFileName, destination);
  copyFile.initMutexAndCondition();
  copyFile.copyFile();
  copyFile.destroyMutexAndCondition();
  // 测试两个文件是否相同
  std::string md5Hash1 = calculateMD5(sourceFileName);
  std::string md5Hash2 = calculateMD5(destination);
  if (md5Hash1 == md5Hash2) {
    std::cout << "相同" << endl;
  } else {
    std::cout << "不相同" << endl;
  }
  return 0;
}