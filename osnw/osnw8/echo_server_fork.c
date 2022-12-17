#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#define MAXLINE 1024
#define PORTNUM 3600

int main(int argc, char *argv[])
{
	int listen_fd, client_fd;
	pid_t pid;
	socklen_t addrlen;
	int readn;
	//int status;
	char res[MAXLINE];
	char buf[MAXLINE];
	char result[MAXLINE];
	struct sockaddr_in client_addr, server_addr;
	int fds1[2], fds2[2];
	if( (listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return 1;
	}
	memset((void *)&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORTNUM);

	if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==-1)
	{
		perror("bind error");
		return 1;
	}
	if(listen(listen_fd, 5) == -1)
	{
		perror("listen error");
		return 1;
	}
	memset(res, 0x00, MAXLINE);
	while(1)
	{
		addrlen = sizeof(client_addr);
		/*
		for(int loop = 0; loop<3; loop++){
			client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addrlen);
			if(client_fd == -1)
                	{
                        	printf("accept error\n");
                        	exit(1);
                	}
                	printf("New Client Connect : %s\n", inet_ntoa(client_addr.sin_addr));
		}
		*/
		client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addrlen);
		if(client_fd == -1)
		{
			printf("accept error\n");
			break;
		}
		printf("New Client Connect : %s\n", inet_ntoa(client_addr.sin_addr));

		if(pipe(fds1)<0){
			exit(1);
		}
		if(pipe(fds2)<0){
			exit(1);
		}
		if((pid = fork()) < 0){
			exit(1);
		}
		if(pid == 0)
		{
			close(listen_fd);
			memset(buf, 0x00, MAXLINE);
			memset(result, 0x00, MAXLINE);
			while((readn = read(client_fd, buf, MAXLINE)) > 0)
			{
				printf("Read Data %s(%d) : %s",inet_ntoa(client_addr.sin_addr),client_addr.sin_port,buf);
				write(fds1[1], buf, MAXLINE);
				read(fds2[0], result, MAXLINE);
				write(client_fd, result, sizeof(result));
				memset(buf, 0x00, MAXLINE);
				memset(result, 0x00, MAXLINE);
			}
			close(client_fd);
			exit(0);
		}
		else if( pid > 0){
			close(client_fd);
			memset(buf, 0x00, MAXLINE);
			read(fds1[0], buf, MAXLINE);
			buf[strlen(buf)-1] = ' ';
			strcat(res,buf);
			write(fds2[1], res, MAXLINE);
		}
	}
	exit(0);
}
