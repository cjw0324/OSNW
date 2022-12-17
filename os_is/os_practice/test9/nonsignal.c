#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){
	int pid = fork();
	if(pid == 0){
		printf("In Child.\n");
	}
	else{
		wait(NULL);
		printf("In Parent.\n");
	}
}
