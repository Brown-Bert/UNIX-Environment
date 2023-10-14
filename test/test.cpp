#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

#include "copyfile.h"
using namespace std;
std::string sourceFileName =
    "/home/yzx/vscode/workspace/unixenv/src/thread/test.txt";
std::string destination =
    "/home/yzx/vscode/workspace/unixenv/src/thread/testcopy.txt";

TEST(COMPARETEST, COMPARE) {
  EXPECT_EQ(true, compare(sourceFileName, destination));
}