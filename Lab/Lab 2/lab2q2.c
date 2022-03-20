#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <sys/sem.h>
#include <signal.h>

#define size 5 //Define buffer size ----- USER DEFINED, Modify Here
int sleep_time = 10; //main() function sleep time ---- USER DEFINED, Modify Here
int pro_thread_num = 1; // Producer thread number ---- USER DEFINED, Modify Here
int con_thread_num = 1; // Consumer thread number ---- USER DEFINED, Modify Here


int buffer[size]; // Buffer Initialization
int pos = 0;// The position Producer/consumer is at inside the buffer

int remove_idx; // The index that the item is removed from
int insert_idx; // The index that the item is inserted to


//Semaphore Initialization
sem_t empty;
sem_t full;
sem_t mutex;

//Signal Alarm for main() function to terminate the program after sleep
void signal_handler ( int signo )
{
  if (signo == SIGALRM)
   {
     printf("main() function sleeps for %d seconds before terminating.\n", sleep_time);
     printf("There are %d producer threads.\n", pro_thread_num);
     printf("There are %d producer threads.\n", con_thread_num);

     sem_destroy(&empty);
     sem_destroy(&full);
     sem_destroy(&mutex);
     exit(0);
   }
}
//Function to insert items to the buffer
int insert_item(int pos)
{
  int value = rand() % 100;
  insert_idx = pos;
  buffer[pos] = value;
  return value;
}
//Function to remove items from the buffer
int remove_item(int pos)
{
  int x;
  remove_idx = pos;
  x = buffer[pos];
  return x;
}
//Producer Thread
void *th_producer(void* pro_id)
{
  int y;
  while(1)
    {
      sem_wait(&mutex);
      sem_wait(&empty);
      sleep (1);
      y = insert_item(pos);
      printf("Producer thread ID: %d -- Inserted item %d to position %d\n",*((int *)pro_id), y, insert_idx);
      pos+=1;
      if (pos == size + 1)
       {pos = size -1;}
      sem_post(&full);
      sem_post(&mutex);
    }
}
//Consumer thread
void *th_consumer(void* con_id)
{
  int x;
  while(1)
    {
      sem_wait(&mutex);
      sem_wait(&full);
      x = remove_item(pos);
      printf("Consumer thread ID: %d -- Removed item %d from position %d\n",*((int *)con_id), x, remove_idx);
      pos-=1;
      if (pos == -1)
       {pos = 0;}
      sem_post(&empty);
      sem_post(&mutex);
      sleep (1);
    }
}

int main(int argc, char* argv[])
{
  //time_t start, end;
  sem_init(&empty, 0, size);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);
  pthread_t producer[pro_thread_num]; //Producer thread initialization
  pthread_t consumer[con_thread_num]; //Consumer thread initialization
  int proarr[pro_thread_num]; //Initialize array to store producer thread ID
  int conarr[con_thread_num]; //Initialize array to store consumer thread ID
  for(int i = 0; i < pro_thread_num; i++)
    {proarr[i] = (i+1);}
  for(int i = 0; i < con_thread_num; i++)
    {conarr[i] = (i+1);}

  signal(SIGALRM, signal_handler); //Register signal handler for alarm
  alarm(sleep_time); //Terminate the program after specified sleep time


  for (int i = 0; i < pro_thread_num; i++)
    {
      pthread_create(&producer[i], NULL, (void*)th_producer, (void*)&proarr[i]);
    }
  for (int j = 0; j < con_thread_num; j++)
    {
      pthread_create(&consumer[j], NULL, (void*)th_consumer, (void*)&conarr[j]);
    }

  //time(&start);

  for (int i = 0; i < pro_thread_num; i++)
    {
      pthread_join(producer[i], NULL);
    }
  for (int i = 0; i < con_thread_num; i++)
    {
      pthread_join(consumer[i], NULL);
    }
  return 0;
}
