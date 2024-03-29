#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAXLINE 1024
#define PORTNUM 3600
#define SOCK_SETSIZE 1024

struct data {
	char str[MAXLINE];
};


int main(int argc, char **argv)
{
	int listen_fd, client_fd;
        socklen_t addrlen;
	struct sockaddr_in server_addr, client_addr;
	int readn;
        int fd_num, sockfd;
        int maxfd = 0, i = 0;
	fd_set readfds, allfds;
        struct data buf;
	struct data result;
        struct data *connect_node[SOCK_SETSIZE];
	struct timeval timeout;
	time_t t;
	struct tm *lt = malloc(sizeof(struct tm));
	memset(&result, 0x00, sizeof(struct data));

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	for(i =0; i < SOCK_SETSIZE; i++)
	{
		connect_node[i] = NULL;
	}

	if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket error");
		return 1;
	}

	memset((void *)&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORTNUM);

	if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("bind error");
		return 1;
	}

	if(listen(listen_fd, 5) == -1)
	{
		perror("listen error");
		return 1;
	}
	FD_ZERO(&readfds);
	FD_SET(listen_fd, &readfds);

	maxfd = listen_fd;

	while(1){
		allfds = readfds;
		fd_num = select(maxfd + 1, &allfds, (fd_set *)0, (fd_set *)0, &timeout);

		//새로운 클라이언트 요청이 있는 경우(앞의 if문 부분은 수행 X)
		if(FD_ISSET(listen_fd, &allfds))
		{
			addrlen = sizeof(client_addr);
			client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addrlen);

			connect_node[client_fd] = malloc(sizeof(struct data));
			memset(connect_node[client_fd], 0x00, sizeof(struct data));

			FD_SET(client_fd, &readfds);

			if(client_fd > maxfd)
			{
				maxfd = client_fd;
			}

			printf("Accept: %s (%d)\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
			continue;	//continue에 의해 다음 흐름으로 넘어가지 않고 바로 while(1)의 처음으로
		}

		//앞의 if문에서 걸리지 않은 경우로 기존에 연결된 클라이언트로부터 입력이 있는 경우이다.
		for (i = 0; i <= maxfd; i++)
                {
                        sockfd = i;
                        if (FD_ISSET(sockfd, &allfds))
                        {
                                memset(&buf, 0x00, sizeof(struct data));
				memset(lt, 0x00, sizeof(struct tm));
				if(read(sockfd, &buf, sizeof(struct data)) <= 0)
				{
					free(connect_node[sockfd]);
					connect_node[sockfd] = NULL;
					FD_CLR(sockfd, &readfds);
					close(sockfd);
					continue;
				}
				else
				{
					read(sockfd, lt, sizeof(struct tm));

					printf("from client %d: %s %s", sockfd, buf.str, asctime(lt));
					strcpy(connect_node[sockfd]->str, buf.str);
                               		strcpy(result.str, connect_node[sockfd]->str);
					for(int j = 0; j < SOCK_SETSIZE; j++)
                        		{
                                		if(connect_node[j] != NULL)
                                		{
							printf("To : %d What : %s When :  %s", j, result.str, asctime(lt));
                                        		write(j, &result, sizeof(struct data));
							write(j, lt, sizeof(struct tm));
                                		}
                        		}
                        		sleep(1);
					continue;
				}
                                if (--fd_num <= 0){
                                        break;
				}
                        }
                }

		memset(&result, 0x00, sizeof(struct data));
	}
}
