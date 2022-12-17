#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *input;
}



int main(void)
{

    struct data work1;
    work1.a = 10;
    work1.b = 10;
    struct data work2;
    work2.a = 1;
    work2.b = 2;
    struct data work3 = { 
        .a = 3, 
        .b = 4
    };


    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    pool_submit(&add,&work1);
    pool_submit(&add,&work2);
    pool_submit(&add,&work3);

    sleep(5);
    pool_shutdown();

    return 0;
}
