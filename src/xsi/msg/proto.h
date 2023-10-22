#ifndef PROTO_H_
#define PROTO_H_

#define PATHNAME "./test.txt"
#define PROJ 'a'
#define NAMESIZE 32

struct data {
  long mtype;
  char name[NAMESIZE];
  int math;
  int chinese;
};

#endif