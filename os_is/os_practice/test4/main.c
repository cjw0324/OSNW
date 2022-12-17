#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

	pid_t pid, ppid;
	pid = getpid();
	ppid = getppid();
	printf("프로세스 PID = %d, 부모 프로세스 PID = %d\n", pid, ppid);

	return 0;


}
