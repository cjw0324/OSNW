#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main(){
	pid_t pid;
	int status;
	pid = fork();
	if(pid>0){
		sleep(1);
		return 0;
	}
	else if(pid==0){
		sleep(2);
		printf("parent PID : %d\n", getppid());
		return -1;
	}
}
