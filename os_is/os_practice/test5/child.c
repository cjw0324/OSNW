#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

int main(){

	printf("i am child, pid = %d\n", getpid());
	return 3;
}

