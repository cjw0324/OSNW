#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3
#define TRUE 1

typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task workqueue[QUEUE_SIZE];

int front;  // front of the queue
int rear;   // rear of the queue
int count;  // number of elements in the queue

// the worker bees
pthread_t tid[NUMBER_OF_THREADS];
sem_t sem;
pthread_mutex_t mutex;

task dequeue() 
{
    task t;


}
 

void *worker(void *param)
{
    while (TRUE) {
        printf("%lu awaiting a task ....\n", pthread_self());
        //1. sem에 대한 p연산 코드

        printf("%lu got a task to do\n", pthread_self());
        //2. work queue로 부터 작업하나 가져오는 dequeue 정의
        task work = dequeue();
        execute(work.function, work.data);
        usleep(10000);
        pthread_testcancel();
    }

    pthread_exit(0);
}

void execute(void (*func)(void *param), void *param)
{
    //3. func을 param을 인자로 하여 실행하는 코드

}

// workqueue
int enqueue(task t) 
{
    int ret_val = 0;
    //3. workqueue
    //4. count < QUEUE_SIZE 
    //     rear and mod QUEUE_SIZE
    //     count  
    //     ret_val
    //5. mutext lock
    //6. ret_val

}

int pool_submit(void (*somefunction)(void *p), void *p)
{
    task t;
    //1.  

    //2.enqueue(t)
    //and return QUEUE_SUCCESS 
    //return QUEUE_REJECTED
    if (enqueue(t))
    {

    }

}


void pool_init(void)
{


}

void pool_shutdown(void)
{

    for (int i = 0; i < NUMBER_OF_THREADS; i++) 
    {
        pthread_cancel(tid[i]);
    }
}
