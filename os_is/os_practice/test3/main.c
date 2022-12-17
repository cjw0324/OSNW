#include <stdio.h>
#include <unistd.h>

int main(){
	printf("0\n");
	fork();
	printf("1\n");
	fork();
	printf("2\n");
	fork();
	printf("3\n");
}
