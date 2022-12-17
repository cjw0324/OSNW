#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int x = 10;
int main(){
	pid_t pid;
	pid = fork();
	if(pid == 0){
		x += 5;
		return 0;
	}else if(pid > 0){
		wait(NULL);
		printf("IN PARENT x=%d\n", x);
		return 0;
	}
}
