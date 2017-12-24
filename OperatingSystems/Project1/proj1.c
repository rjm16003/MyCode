/* Created by: Robert McNeil
 * Created on: February 24, 2017
 * Last Modified: February 25, 2017
 * Class: CS4348.001 Operating Systems
 *
 * Project 1:
 * Utilizing forks and pipes, simulate a basic computer with
 * a CPU and Memory
 *
 * To run:
 * compile with gcc
 * ./a.out filename timerlimit
 * Where filename is the name of the file to be run
 * Where timerlimit is the limit until an interrupt occurs
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <math.h>
#include <string.h>

void cpu(int *, int *, int);
void memory(int *, int *, char *);

int  timerlimit = 8;//timer limit for interrupt
char * filename;

int main(int argc, char**argv)
{
   timerlimit = atoi(argv[2]);//get the interrupt timer limit
   filename = argv[1];//get the file name
   if(argc != 3){//if we are missing an arg, toss an error and exit
      printf("ERROR! Need to supply filename and timerlimit: ./a.out sample1.txt 20\n");
      exit(-1);
   }


   int fd1[2];//CPU READS, MEM WRITES
   int fd2[2];//CPU WRITES, MEM READS
   int result;
   result = pipe(fd1);//create the two pipes
   result = pipe(fd2);

   pid_t pid;
   pid = fork();//fork for two processes

   switch(pid) {
      case -1:
         printf("Cannot fork\n");
	 break;
      
      case 0://child in charge of memory
         close(fd1[0]);//CONTROLS MEMORY
	 close(fd2[1]);
	 memory(fd1, fd2, filename);
	 close(fd1[1]);
	 close(fd2[0]);
	 break;

      default://CONTROLS CPU
         close(fd1[1]);//close the pipes so that they are one way
	 close(fd2[0]);
	 cpu(fd1, fd2, timerlimit);
	 close(fd1[0]);
	 close(fd2[1]);
	 break;
   }


   return 0;
}

void memory(int * fd1, int * fd2, char * filename){
   int MEMORY[2000];
   int counter = 0;


   FILE *fp;
   size_t count;
   char buffer[128];

   fp = fopen(filename, "r");//open the file

//   fp = fopen("sample.txt", "r");
   if (fp == NULL){
      printf("Failed to open the file\n");
   }

   int input = 0;

   while(fgets(buffer,128,fp) != NULL){//go through the instructions
      if(buffer[0]=='.') {//if we find a ., go to that line in memory
         char * buff = buffer;
         sscanf(++buff, "%d", &input);
	 counter = input;
      }
      else{
         int result = sscanf(buffer, "%d", &input);//if we find a number
	 if(result>0){
	    MEMORY[counter++] = input; //initalize memory
         }
      }
   }

   fclose(fp);//close the file
   
   char type;
   int add;
   int data;

   while(1){
   read(fd2[0], &type, sizeof(type));//read from the CPU
   if(type== 'R'){//R signals a read
      read(fd2[0], &add, sizeof(add));//read the address
      data = MEMORY[add];//get the data at address value
      write(fd1[1], &data, sizeof(data));//write the data back in the pipe
   }

   if(type == 'W'){//W signals a write
      read(fd2[0], &add, sizeof(add));//read the address
      read(fd2[0], &data, sizeof(data));//read the data
      MEMORY[add] = data;
   }

   if(type == 'E') break;//E signals the end of program, terminate
   }
   

}

void cpu(int * fd1, int * fd2, int timerlimit) {

   int PC = 0;//program counter starts at 0, keeps track of address
   int AC = 0;//accumulator
   int SP = 1000;//stack pointer
   int IR = 0;//instruction register, actual value of address
   int X = 0;//x
   int Y = 0;//y

   int  SYSTEM_MODE = 0;//0 indicates user mode

   size_t size;
   int i;
   const double SENTINEL = -1;
   
   char re = 'R';//read flag
   char wr = 'W';//write flag
   char en = 'E';//end flag
   
   int data;
   int add;

   int end = 0;//0 indicates we have not reached the end
   int timer = timerlimit;
   while(end == 0){
      if(timer<1){//interrupt
         SYSTEM_MODE = 1;

	 write(fd2[1], &wr, sizeof(wr));//indicate a write being done
	 SP--;
	 write(fd2[1], &SP, sizeof(SP));
	 write(fd2[1], &PC, sizeof(PC));//put PC on the user stack
         
	 SP--;
         int tempsp = SP;
	 SP = 1999;

	 write(fd2[1], &wr, sizeof(wr));//
	 write(fd2[1], &SP, sizeof(SP));
	 write(fd2[1], &tempsp, sizeof(tempsp));//put SP on the system stack
	 PC = 1000;//start at system program
	 timer = timerlimit;//reset timer
      }
      if(SYSTEM_MODE == 0) timer--;//if in user mode, decrement timer
         write(fd2[1],&re, sizeof(re));//signal a read
         write(fd2[1],&PC, sizeof(PC));//increment the PC
         PC++;
         read(fd1[0], &IR, sizeof(IR));//read the instruction
      switch(IR){
         case 1://load value
	    write(fd2[1], &re, sizeof(re));//signal a read
	    write(fd2[1], &PC, sizeof(PC));//send address
	    PC++;//increment PC
	    read(fd1[0], &AC, sizeof(AC));//load value of AC
	    break;

	 case 2://load value at address
	    write(fd2[1], &re, sizeof(re));//signal a read
	    write(fd2[1], &PC, sizeof(PC));
	    PC++;
            
	    read(fd1[0], &add, sizeof(add));//get address
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &add, sizeof(add));
	    read(fd1[0], &AC, sizeof(AC));//load value
	    if(SYSTEM_MODE == 0 && add >= 1000){//if access violation
               printf("Memory Violation: accessing system address %d in user mode\n", add);
	       write(fd2[1], &en, sizeof(en));
	       end = 1;//end the program
	    }
	    break;

	 case 3://loadInd Address
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &PC, sizeof(PC));
	    PC++;
	    read(fd1[0], &add, sizeof(add));//get address

	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &add, sizeof(add));
	    read(fd1[0], &add, sizeof(add));//get address

	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &add, sizeof(add));
	    read(fd1[0], &AC, sizeof(AC));//load value
	    if(SYSTEM_MODE == 0 && add >= 1000){
               printf("Memory Violation: accessing system address %d in user mode\n", add);
	       write(fd2[1], &en, sizeof(en));
	       end = 1;
	    }
	    break;

	 case 4://load IDxX address
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &PC, sizeof(PC));
	    PC++;

	    read(fd1[0], &add, sizeof(add));
	    add = add + X;
	    
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &add, sizeof(add));
	    read(fd1[0], &AC, sizeof(AC));
	    if(SYSTEM_MODE == 0 && add >= 1000){
               printf("Memory Violation: accessing system address %d in user mode\n", add);
	       write(fd2[1], &en, sizeof(en));
	       end = 1;
	    }
	    break;

	 case 5://load idxY address
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &PC, sizeof(PC));
	    PC++;

	    read(fd1[0], &add, sizeof(add));
	    add = add + Y;

	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &add, sizeof(add));
	    read(fd1[0], &AC, sizeof(AC));
	    if(SYSTEM_MODE == 0 && add >= 1000){
               printf("Memory Violation: accessing system address %d in user mode\n", add);
	       write(fd2[1], &en, sizeof(en));
	       end = 1;
	    }
	    break;

         case 6://sp + x
	    add = SP + X;
	    
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &add, sizeof(add));
	    read(fd1[0], &AC, sizeof(AC));
	    break;

	 case 7://store
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &PC, sizeof(PC));
	    PC++;
	    read(fd1[0], &add, sizeof(add));//get address

	    write(fd2[1], &wr, sizeof(wr));
	    write(fd2[1], &add, sizeof(add));
	    write(fd2[1], &AC, sizeof(AC));//store the AC value at address
	    break;

	 case 8://get random
            srand(time(NULL));
	    int randnum = rand() % 100 + 1;

	    AC = randnum;
	    break;

	 case 9://put port
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &PC, sizeof(PC));
	    PC++;
	    read(fd1[0], &data, sizeof(data));
            
	    if(data==1){//print as int
               printf("%d", AC);
	    }
	    else if(data == 2) {//print as a char
               printf("%c", AC);
	    }

            break;
	 case 10://AddX
	    AC = X + AC;
            break;

	 case 11://AddY
	    AC = Y + AC;
            break;

	 case 12://SubX
	    AC = AC - X;
	    break;

	 case 13://SubY
	    AC = AC - Y;
	    break;
	 
	 case 14://CopyToX
	    X = AC;
	    break;

	 case 15://CopyFromX
	    AC = X;
	    break;

	 case 16://CopyToY
	    Y = AC;
	    break;

	 case 17://CopyFromY
	    AC = Y;
	    break;

	 case 18://CopyToSp
	    SP = AC;
	    break;

	 case 19://CopyFromSp
	    AC = SP;
	    break;

	 case 20://Jump addr
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &PC, sizeof(PC));
	    PC++;
	    read(fd1[0], &add, sizeof(add));

	    PC = add;
	    if(SYSTEM_MODE == 0 && add >= 1000){
               printf("Memory Violation: accessing system address %d in user mode\n", add);
	       write(fd2[1], &en, sizeof(en));
	       end = 1;
	    }
	    break;

	 case 21://Jump if equal
            write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &PC, sizeof(PC));
	    PC++;
	    read(fd1[0], &add, sizeof(add));
	    if(AC==0){
	       PC = add;
	    if(SYSTEM_MODE == 0 && add >= 1000){
               printf("Memory Violation: accessing system address %d in user mode\n", add);
	       write(fd2[1], &en, sizeof(en));
	       end = 1;
	    }
	    }
	    break;

	 case 22://jump if not equal
            write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &PC, sizeof(PC));
	    PC++;
	    read(fd1[0], &add, sizeof(add));
	    if(AC!=0){
	       PC = add;
	    if(SYSTEM_MODE == 0 && add >= 1000){
               printf("Memory Violation: accessing system address %d in user mode\n", add);
	       write(fd2[1], &en, sizeof(en));
	       end = 1;
	    }
//	       printf("WE ARE JUMPING!! %d   %d\n", PC, add);
	    }
	    break;

	 case 23://call address
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &PC, sizeof(PC));
	    PC++;
	    read(fd1[0], &add, sizeof(add));

	    write(fd2[1], &wr, sizeof(wr));
	    SP--;
	    write(fd2[1], &SP, sizeof(SP));
	    write(fd2[1], &PC, sizeof(PC));

	    PC = add;
	    break;

	 case 24://return
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &SP, sizeof(SP));
	    SP++;
	    read(fd1[0], &add, sizeof(add));


	    PC = add;
	    break;

	 case 25://IncX
	    X = X + 1;
	    break;

	 case 26://DecX
	    X--;
	    break;

	 case 27://Push AC onto Stack
	    write(fd2[1], &wr, sizeof(wr));
	    SP--;
	    write(fd2[1], &SP, sizeof(SP));
	    write(fd2[1], &AC, sizeof(AC));
	    break;

	 case 28://Pop AC from Stack
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &SP, sizeof(SP));
	    SP++;
	    read(fd1[0], &AC, sizeof(AC));
	    break;

	 case 29://perform system call
	    SYSTEM_MODE = 1;

	    write(fd2[1], &wr, sizeof(wr));
	    SP--;
	    write(fd2[1], &SP, sizeof(SP));
	    write(fd2[1], &PC, sizeof(PC));

	    PC = 1500;

	    write(fd2[1], &wr, sizeof(wr));
	    SP--;
	    int temp = SP;
	    SP = 1999;
	    write(fd2[1], &SP, sizeof(SP));
	    write(fd2[1], &temp, sizeof(temp));
	    break;

	 case 30://return from system call
	    SYSTEM_MODE = 0;
	    
	    write(fd2[1], &re, sizeof(re));
	    write(fd2[1], &SP, sizeof(SP));//increment SP
	    SP++;
	    read(fd1[0], &add, sizeof(add));//get the address


	    SP = add;

	    write(fd2[1], &re, sizeof(re));
	    SP++;
	    write(fd2[1], &SP, sizeof(SP));
	    read(fd1[0], &add, sizeof(add));//get the PC

	    PC = add;
	    SP++;
	    break;

	 case 50://END
	    write(fd2[0], &en, sizeof(en));
	    end = 1;
	    break;

      }

   }

}



