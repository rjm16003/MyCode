#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

void writer(int *);
void reader(int *);

int main() {
  int fd[2]; // fd[0] is read, fd[1] is write

  int result;
  result = pipe(fd);

  if (result != 0) {
    fprintf(stderr, "Unable to open bi-directional pipe with errno %d\n", errno);
    exit(EXIT_FAILURE);
  }

  printf("Successfully opened bi-directional pipe\n");

  pid_t pid;
  pid = fork();
  switch(pid) {
  case -1:
    fprintf(stderr, "Cannot fork\n");
    exit(EXIT_FAILURE);
    break;

  case 0:
    close(fd[0]);
    printf("This is the child process (writer)\n");
    writer(fd);
    close(fd[1]);
    break;

  default:
    close(fd[1]);
    printf("This is the parent process (reader)\n");
    reader(fd);
    close(fd[0]);
    break;
  }

  return 0;
}

void writer(int *fd) {
  size_t size;
  int i;

  for (i = 0; i < 10; ++i) {
    double root = sqrt(i);
    printf("Writing to pipe: %f\n", root);
    size = write(fd[1], &root, sizeof(root));
    printf("Wrote %d bytes\n", size);
  }
}

void reader(int *fd) {
  size_t size;
  int i;

  for (i = 0; i < 10; ++i) {
    double root;
    size = read(fd[0], &root, sizeof(root));
    printf("Read %d bytes\n", size);
    printf("Reading from pipe: %f\n", root);
  }
}
