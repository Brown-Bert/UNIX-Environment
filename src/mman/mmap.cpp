#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  struct stat statres;
  char* strs = NULL;
  int count = 0;

  if (argc < 0) {
    fprintf(stderr, "Using......\n");
    exit(1);
  }
  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    perror("open()");
    exit(1);
  }
  int res = fstat(fd, &statres);
  if (res < 0) {
    close(fd);
    perror("fstat()");
    exit(1);
  }
  strs = (char*)mmap(NULL, statres.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  for (int i = 0; i < statres.st_size; i++) {
    if (strs[i] == 'a') count++;
  }
  printf("%d\n", count);
  close(fd);
  munmap(strs, statres.st_size);
  exit(0);
}