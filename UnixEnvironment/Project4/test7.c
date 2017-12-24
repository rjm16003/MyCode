#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

void writer(int *);
void reader(int *);
void primes(int, int, int*);

const double SENTINEL = -1;


int main() {
  int fd[2]; // fd[0] is read, fd[1] is write

  int result;
  result = pipe(fd);

  if (result != 0) {
    fprintf(stderr, "Unable to open bi-directional pipe with errno %d\n", errno);
    exit(EXIT_FAILURE);
  }

  printf("Successfully opened bi-directional pipe\n");

  int ix=0;
  
//  primes(2,1000, fd);


//  for(ix=1; ix<2; ix++){

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
    primes(1001, 100000, fd);
    writer(fd);
    close(fd[1]);
    break;

  default:
    close(fd[1]);
    printf("This is the parent process (reader)\n");
    reader(fd);
//    primes(10000*ix + 1, 100000*ix + 100000, fd);
    close(fd[0]);
    break;
  }
  
//  if (pid==0){
//     break;
//  }

//  }

  return 0;
}

void writer(int *fd) {
  size_t size;
  int i;

  for (i = 0; i < 1; ++i) {
    double root = sqrt(i);
//    printf("Writing to pipe: %f\n", root);
    size = write(fd[1], &root, sizeof(root));
    printf("Wrote %d bytes\n", size);
  }

  printf("Writing to pipe: %f\n", SENTINEL);
  size = write(fd[1], &SENTINEL, sizeof(SENTINEL));
  printf("Wrote %d bytes\n", size);

}

void reader(int *fd) {
  size_t size;
  int i;

  while(1) {
    double root;
    size = read(fd[0], &root, sizeof(root));
//    printf("Read %d bytes\n", size);
    if(root >  1) printf("Reading from pipe: %f\n", root);
    if(root == SENTINEL){
       break;
    }
  }
}


void primes(int start, int end, int* fd){
   printf("%d, %d", start, end);
   int isPrime = 1;
   size_t size;
   for(start; start<=end; start++){
      int iy=2;
      for(iy; iy<start; iy++){
         if(start%iy==0) isPrime = 0;
      }
      if(isPrime==1){
         printf("%d\n", start);
	 size = write(fd[1], &start, sizeof(start));
      }
      else isPrime=1;
   }
   size = write(fd[1], &SENTINEL, sizeof(SENTINEL));
}

