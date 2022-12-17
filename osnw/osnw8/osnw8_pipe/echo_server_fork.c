#include <fcntl.h>
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
	char res[MAXLINE];
	char buf[MAXLINE];
	char result[MAXLINE];
	struct sockaddr_in client_addr, server_addr;
	int rfd, wfd;
	int sc_rfd, sc_wfd;
	mkfifo("/tmp/myfifo_r", S_IRUSR|S_IWUSR);
	mkfifo("/tmp/myfifo_w", S_IRUSR|S_IWUSR);
	mkfifo("/tmp/myfifo_scr", S_IRUSR|S_IWUSR);
        mkfifo("/tmp/myfifo_scw", S_IRUSR|S_IWUSR);
	if( (rfd = open("/tmp/myfifo_r", O_RDWR)) == -1)
        {
                perror("rfd error");
                return 1;
        }
        if ( (wfd = open("/tmp/myfifo_w", O_RDWR)) == -1)
        {
                perror("wfd error");
                return 2;
        }
	if( (sc_rfd = open("/tmp/myfifo_scr", O_RDWR)) == -1)
        {
                perror("sc_rfd error");
                return 3;
        }
        if ( (sc_wfd = open("/tmp/myfifo_scw", O_RDWR)) == -1)
        {
                perror("sc_wfd error");
                return 4;
        }
	if( (listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return 5;
	}
	memset((void *)&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORTNUM);

	if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==-1)
	{
		perror("bind error");
		return 6;
	}
	if(listen(listen_fd, 5) == -1)
	{
		perror("listen error");
		return 7;
	}
	memset(res, 0x00, MAXLINE);
	while(1)
	{
		addrlen = sizeof(client_addr);
		client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addrlen);
		if(client_fd == -1)
		{
			printf("accept error\n");
			return 8;
		}
		printf("New Client Connect : %s\n", inet_ntoa(client_addr.sin_addr));
		if((pid = fork()) < 0)
		{
			return 9;
		}
		if(pid == 0)
		{
			close(listen_fd);
			memset(buf, 0x00, MAXLINE);
			memset(result, 0x00, MAXLINE);
			if((readn = read(sc_rfd, buf, MAXLINE)) > 0)
			{
				printf("Read Data %s(%d) : %s",inet_ntoa(client_addr.sin_addr),client_addr.sin_port,buf);
				write(rfd,buf,MAXLINE);
				read(wfd,result,MAXLINE);
				write(sc_wfd, result, sizeof(result));
				memset(buf, 0x00, MAXLINE);
				memset(result, 0x00, MAXLINE);
			}
			close(client_fd);
			close(wfd);
			close(rfd);
			exit(0);
		}
		else if( pid > 0){
			close(client_fd);
			memset(buf, 0x00, MAXLINE);
			read(rfd, buf, MAXLINE);
			buf[strlen(buf)-1] = ' ';
			strcat(res,buf);
			write(wfd, res, MAXLINE);
		}
	}
	exit(0);
}
