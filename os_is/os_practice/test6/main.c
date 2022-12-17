#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int x = 10;
int main(){
	int y = 10;
	pid_t pid;
	pid = fork();
	if(pid == 0){
		x+=5;
		y+=5;
		printf("IN CHILD x = %d, y = %d\n", x, y);
		return 0;
	}
	else if(pid>0){
		wait(NULL);
		printf("IN PARENT x = %d, y = %d\n", x, y);
		return 0;
	}
}
