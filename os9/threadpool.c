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
        //1. 세마포 sem에 대한 P연산 코드 삽입

        printf("%lu got a task to do\n", pthread_self());
        //2. work queue로부터 작업 하나 가져오는 dequeue 정의
        task work = dequeue();
        execute(work.function, work.data);
        usleep(10000);
        pthread_testcancel();
    }

    pthread_exit(0);
}

void execute(void (*func)(void *param), void *param)
{
    //3. func을 param을 인자로하여 실행하는 코드 삽입

}

// workqueue에 t 삽입 시도 
// 성공시 1 리턴, 꽉 차서 실패하면 0 리턴
int enqueue(task t) 
{
    int ret_val = 0;
    //3. workqueue에 대한 배타적 접근을 위한 mutex lock
    //4. count < QUEUE_SIZE 일 경우 rear 인덱스 위치에 t 삽입
    //     rear 인덱스 증가 and mod QUEUE_SIZE
    //     count 증가 
    //     ret_val에 1 삽입
    //5. mutext lock 해제
    //6. ret_val 리턴

}

int pool_submit(void (*somefunction)(void *p), void *p)
{
    task t;
    //1. t에 func와 param 정보 삽입하기 

    //2.enqueue(t)가 성공 (1) 하면 세마포 sem에 대한 V연산
    //and return QUEUE_SUCCESS 
    //실패하면 return QUEUE_REJECTED
    if (enqueue(t))
    {

    }

}


void pool_init(void)
{
    //1. mutext 초기화 
    //2. 세마포 sem 초기화 (count = 0)
    //3. NUMBER_OF_THREADS 만큼 스레드 생성 
    //4. start routine : worker
}

//worker thread를 취소함
//cancellation points : 
//pthread_testcancel(), pthread_join(), sem_wait(),..
//worker() 함수 안에서 execute 실행한 후에 pthread_testcancel()을 실행
void pool_shutdown(void)
{

    for (int i = 0; i < NUMBER_OF_THREADS; i++) 
    {
        pthread_cancel(tid[i]);
    }
}
