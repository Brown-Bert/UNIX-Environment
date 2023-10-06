#include <stdio.h>
#include <stdlib.h>

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

#include "copyfile.h"
#include "filesystem.h"
int main() {
  InitFileSystem();
  string input;
  string option;
  string name;
  while (1) {
    printf("yzx-filesystem$ ");
    getline(cin, input);
    if (input == "exit") {
      printf("do you want to delete system [Y/n? ]");
      getline(cin, input);
      if (input == "Y" || input == "y") {
        system("rm systemfile");
        break;
      }
      break;
    }
    int pos = input.find(" ");
    if (pos < 0) {
      option = input;
    } else {
      option = input.substr(0, pos);
      name = input.substr(pos);
    }
    menu(option, name);
    // cout << "option " << option << endl;
    // cout << "name " << name << endl;
  }
  //   mymkfile("123");
  //   fileinfo("ll");
  //   deleteFile("wzf");
  //   printInodeBitMap();
  //   fileinfo("ll");
  //   printDataBitMap();
  exit(0);
}