#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	pid_t pid;
	
	printf("L0\n");
	pid = fork();
	
	if(pid > 0){
		printf("parent1\n");
		return 0;	
	}
	else if(pid ==  0){
		printf("child1\n");
		return 0;
	}
	printf("L1\n");

	return 0;

}
