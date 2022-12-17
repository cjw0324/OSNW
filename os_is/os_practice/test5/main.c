#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

void main(void){
	pid_t pid;
	pid = fork();
	int status;
	if(pid > 0){
		printf("parent, pid = %d\n", getpid());
		pid = wait(&status);
		printf("pid = %d, status = %d, child exitcode : %d\n", pid, status, WEXITSTATUS(status));
		if(WIFEXITED(status) != 0){
			printf("WIFEXITED(status) = %d, 정상 종료 됨.\n", WIFEXITED(status));
		}
	}
	else if(pid == 0){
		printf("this is main, child pid = %d, parent pid = %d\n", getpid(), getppid());
		execlp("./child", "child", NULL);
	}
	exit(0);
}
