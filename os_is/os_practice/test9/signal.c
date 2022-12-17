#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

static void reapChild(int unused){
	printf("reapChild\n");
	wait(NULL);
}

int main(){
	int pid = fork();
	if(pid == 0){
		printf("In Child\n");
	}
	else{
		printf("In Parent.\n");
		wait(NULL);
		signal(SIGCHLD,reapChild);
	}
}
