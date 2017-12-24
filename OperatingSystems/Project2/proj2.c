#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>


#define CTHREADS 50
#define WTHREADS 3

/* Design Phase:
 * semaphore max_capacity = 10;
 * semaphore worker_station = 3;
 * semaphore scales = 1;
 * int count;
 * semaphore mutex1 = 1, mutex2 = 1;
 * semaphore cust_ready = 0, leave_w_station = 0;
 * semaphore finished[50] = {0};
 *
 * Customer Thread:
 * int custnr;
 * wait(max_capacity);
 * enter_store();
 * wait(mutex1)//prevent other processes from taking num
 * count++;
 * custnr = count;
 * signal(mutex1);
 * wait(worker_station);
 * go_to_station();
 * wait(mutex2);
 * enqueue1(custnr);
 * signal(cust_ready);
 * signal(mutex2);
 * wait(enquiry)
 * give_order();
 * signal(fenquiry);
 * wait(finished[custnr]);
 * leave_station();
 * signal(leave_w_station);
 * exit_store();
 * signal(max_capacity);
 * 
 * 
 * Worker Thread:
 * int b_cust;
 * while(true){
 *    wait(cust_ready);
 *    wait(mutex2);
 *    dequeue1(b_cust);
 *    signal(mutex2);
 *    greet();
 *    signal(enquiry);
 *    wait(fenquiry);
 *    serve();
 *    signal(finished[b_cust]);
 *    wait(leave_w_station);
 *    signal(worker_station);
 * }
 * 
 */



void *customerThread(void*);
void *workerThread(void*);

sem_t max_capacity;//10
sem_t worker_station;//3
sem_t scales;//1

sem_t mutex1;//1
sem_t mutex2;//1
sem_t cust_ready;//0
sem_t leave_w_station;//0

sem_t enquiry;//1
sem_t fenquiry;//1

sem_t finished[CTHREADS+1];//1

int count = 0;
int workerwait = 1;


pthread_t cthreads[CTHREADS];
pthread_t wthreads[WTHREADS];


int cnum[CTHREADS];//customer queue
int stype[CTHREADS];//type of customer service order
int wnum[CTHREADS];//worker number array

int front = 0;
int cfront = 0;
int rear = -1;
int iCount = 0;

void enqueue(int custnum, int servetype){//enqueue operation
   if(!iCount == CTHREADS);{
      if(rear == CTHREADS-1) rear = -1;
      cnum[++rear] = custnum;
      stype[rear] = servetype;
      iCount++;
   }
}

int dequeue(int queue[]){//dequeue operation
   int data = queue[front];
   cfront++;
   if(cfront%2 ==0) {
      front++;
//      printf("Added to front, %d\n", front);
   }
   if(front == CTHREADS) front = 0;
   iCount--;
   return data;
}

void printarray(int arr[]){//print a queue
   int ix=0;
   int max=CTHREADS;
   for(ix=0; ix<max; ix++){
      printf("%d ", arr[ix]);
   }
}

int main(){
   pthread_t tid;
   
   int status;
   int customer;
   int worker;

   int errcode;

   int cids[CTHREADS];
   int wids[WTHREADS];

   //initaize semaphores
   sem_init(&max_capacity, 0, 10);
   sem_init(&worker_station, 0, WTHREADS);
   sem_init(&scales, 0, 1);

   sem_init(&mutex1, 0, 1);
   sem_init(&mutex2, 0, 1);
   sem_init(&enquiry, 0, 0);
   sem_init(&fenquiry, 0, 0);
   sem_init(&cust_ready, 0, 0);
   sem_init(&leave_w_station, 0, 1);
   for(customer=0; customer<CTHREADS; customer++){
      sem_init(&finished[customer], 0, 0);
   }

   //create the workers
   for(worker=0; worker<WTHREADS; worker++){
      wids[worker] = worker;
      pthread_create(&wthreads[worker],NULL,workerThread,&wids[worker]);
   }
   
   //create the customers
   for(customer=0; customer<CTHREADS; customer++){
      cids[customer] = customer;
      pthread_create(&cthreads[customer],NULL,customerThread,&cids[customer]);

   }
   
   //join customers
   for(customer=0; customer<CTHREADS; customer++){
      pthread_join(cthreads[customer], (void**) &status);
   }
   workerwait = 0;

   //join workers
   for(worker=0; worker<WTHREADS; worker++){
      printf("Worker %d Joined\n", worker);
//      pthread_join(wthreads[worker], (void **) &status);
   }

   return 0;
}


void* customerThread(void* input){
   int myid =*(int *) input;
   printf("Customer %d created\n",myid);

   int result = (rand() % 3 + 1);//randomly generate a task

   sem_wait(&max_capacity);
   printf("Customer %d enters the post office\n",myid);
   
   sem_wait(&mutex1);
   sem_post(&mutex1);

   sem_wait(&worker_station);//wait for an open station
   sem_wait(&mutex2);
   enqueue(myid, result);//enque customer number and task
   sem_post(&cust_ready);
   sem_post(&mutex2);
   sem_wait(&enquiry);
   int workerid = wnum[myid];
   if(result == 1) printf("Customer %d asks postal worker %d to buy stamps\n", myid, workerid);
   if(result == 2) printf("Customer %d asks postal worker %d to mail a package\n", myid, workerid);
   if(result == 3) printf("Customer %d asks postal worker %d to mail a letter\n", myid, workerid);
   sem_post(&fenquiry);
   sem_wait(&finished[myid]);//wait to for worker to finish
   
   sem_post(&leave_w_station);//leave station
   printf("Customer %d leaves post office\n", myid);
   sem_post(&max_capacity);//leave post office
   
   pthread_join(cthreads[myid], NULL);
   printf("Joined customer %d\n", myid);

   return NULL;
}

void* workerThread(void* input){
   int myid=*(int *) input;
   int b_cust;
   printf("Postal worker %d created\n", myid);
   while(workerwait == 1){
      sem_wait(&cust_ready);//wait for customer
      
      sem_wait(&mutex2);
      int custnr2 = dequeue(cnum);//get customer number
      int service = dequeue(stype);//get service type
      wnum[custnr2] = myid;
      printf("Postal worker %d serving customer %d\n", myid, custnr2);
      sem_post(&mutex2);
      sem_post(&enquiry);
      sem_wait(&fenquiry);
      if(service==1) {
	 usleep(1000000);//sleep a second
      }
      if(service==3) {
         usleep(1500000);//sleep 1.5 seconds
      }
      if(service==2) {
         sem_wait(&scales);//wait on scales
         printf("Scales in use by postal worker %d\n", myid);
         usleep(2000000);//sleep 2 seconds
	 printf("Scales released by postal worker %d\n", myid);
	 sem_post(&scales);//release scales
      }
      printf("Worker %d finished serving %d\n", myid, custnr2);
      sem_post(&finished[custnr2]);//release customer
      sem_wait(&leave_w_station);//wait for customer to leave
      
      sem_post(&worker_station);//signal an open station

   }

   return input;
}



