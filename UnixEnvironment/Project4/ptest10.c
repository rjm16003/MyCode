//Project 4 for CS 3376.002
//Programmer: Robert McNeil
//NetID: rjm160030
//Description: Using forks and piping, the prime numbers from 2
//to 1,000,000 are calculated.
//
//NOTE: Because the math library is used,
//      need to use gcc -lm ptest10.c
////////////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

void writer(int *, int); //write to pipes
void reader(int *); //read the pipes and display to screen
void primes(int, int, int*); //calc the primes

const double  SENTINEL = -1; //end of pipe value
const int NUM_PROCS = 11; //number of processes

int main() {
  
  int pipes[NUM_PROCS][2]; //2d array of pipes
  int i=0;
  for(i=0; i< NUM_PROCS; i++){ //generate the pipes
     if((pipe(pipes[i])) < 0) printf("Error opening the pipe\n");
  }//if an error occurs, print error

  for(i=0; i< NUM_PROCS; i++){ //for each pipe

  pid_t pid;
  pid = fork(); //fork
  switch(pid) {
  case -1:
    fprintf(stderr, "Cannot fork\n");
    exit(EXIT_FAILURE);
    break;

  case 0: //the child writes
    close(pipes[i][0]);
    writer(pipes[i], i);
    close(pipes[i][1]);
    break;

  default://the parent can not write
    close(pipes[i][1]);
    break;
  }
  
  if (pid==0){ //if it is the child
      break; //break so we do not spawn more processes from the child
      }
   else if(i==(NUM_PROCS-1)) {//once all the tasks have been generated
      int ix=0;// for each pipe, read from pipe
      for(ix=0; ix<NUM_PROCS; ix++) reader(pipes[ix]);
      }
  }

  return 0;
}

void writer(int *fd, int ix) {
  size_t size;
  int i;
  
  if(ix==0){//if first iteration, look for primes from 2 to 1001
    primes(2, 1001, fd);
  }
  
  else if(ix==1){//if second iteration, look for primes from 1001 to 100000
    primes(1001,100000,fd);
  }

  else{
    int start = 100000*(ix-1) + 1;
    int end = 100000*(ix);
    primes(start, end, fd);
  }

  //signal that we have reached the end of what we want to write
  size = write(fd[1], &SENTINEL, sizeof(SENTINEL));

}

void reader(int *fd) {
  size_t size;
  int i;

  while(1) {//while we have not found the end signal
    double  root;
    //continue reading from the pipe
    size = read(fd[0], &root, sizeof(root));
    if(root >  1) printf("%.0f\n", root);
    if(root == SENTINEL){
       break;
    }
  }
}

void primes(int start, int  end, int* fd){
   int isPrime = 1;//1= the num is prime, 0 = the num is not prime
   size_t size;
   for(start; start<=end; start++){//search through the numbers
      int iy=2;
      for(iy; iy<=sqrt(start); iy++){
        if(start%iy==0) isPrime = 0;//if we found a divisble match, not prime
      }
      if(isPrime==1){//if is prime
         double item = start;//write to the pipe
	 size = write(fd[1], &item, sizeof(item));
      }
      else isPrime=1;//reset isPrime to 1
   }
}



