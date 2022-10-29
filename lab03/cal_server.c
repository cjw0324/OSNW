
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 3600
#define MAXLINE 1024
struct cal_data
{
        int left_num;
        int right_num;
        char op;
        int result;
        short int error;
};
struct result_data
{
	int maxs;
        int mins;
        char max_id[MAXLINE];
        char min_id[MAXLINE];
};

int main(int argc, char *argv[])
{
	int client_sockfd[3];
        struct sockaddr_in client_addr, sock_addr;
        int listen_sockfd;
        int addr_len, loop;
        struct cal_data rdata;
        int left_num, right_num, cal_result;
        short int cal_error;
	int max = 0;
	int min = 0;
	struct result_data return_data;
        if( (listen_sockfd  = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
                perror("Error ");
                return 1;
        }

        memset((void *)&sock_addr, 0x00, sizeof(sock_addr));
        sock_addr.sin_family = AF_INET;
        sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        sock_addr.sin_port = htons(PORT);

        if( bind(listen_sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1)
        {
                perror("Error ");
                return 1;
        }

        if(listen(listen_sockfd, 5) == -1)
        {
                perror("Error ");
                return 1;
        }
	while(1)
	{
		for(loop = 0; loop < 3; loop++){
                        client_sockfd[loop] = accept(listen_sockfd, (struct sockaddr *)&client_addr, &addr_len);
                        printf("New Client[%d] Connect: %s\n", loop, inet_ntoa(client_addr.sin_addr));
                	read(client_sockfd[loop], (void *)&rdata, sizeof(rdata));
			printf("%d %d %c\n",htonl(rdata.left_num), htonl(rdata.right_num), htonl(rdata.op));
			cal_result = 0;
                	cal_error = 0;

                	left_num = ntohl(rdata.left_num);
                	right_num = ntohl(rdata.right_num);

                	switch(rdata.op)
                	{
                        	case '+':
                                	cal_result = left_num + right_num;
                                	break;
                        	case '-':
                                	cal_result = left_num  - right_num;
                                	break;
                        	case '*':
                                	cal_result = left_num * right_num;
                                	break;
                        	case '/':
                                	if(right_num == 0)
                                	{
                                        	cal_error = 2;
                                        	break;
                                	}
                                	cal_result = left_num / right_num;
                                	break;
                        	default:
                                	cal_error = 1;

                	}
                	rdata.result = htonl(cal_result);
                	rdata.error = htons(cal_error);
                	printf("%d\n",cal_result);
			if(loop == 0){
				max = cal_result;
				strcpy(return_data.max_id, inet_ntoa(client_addr.sin_addr));
			}
			else{
				if(max < cal_result){
					//printf("max is change %d to %d\n", max, cal_result);
					max = cal_result;
					strcpy(return_data.max_id, inet_ntoa(client_addr.sin_addr));
				}
			}

			if(loop == 0){
				min = cal_result;
				strcpy(return_data.min_id, inet_ntoa(client_addr.sin_addr));
			}
			else{
				if(min > cal_result){
					//printf("min is change %d to %d\n", min, cal_result);
					min = cal_result;
					strcpy(return_data.min_id, inet_ntoa(client_addr.sin_addr));
				}

			}

			return_data.maxs = max;
                        return_data.mins = min;

			printf("min=%d from %s\n", return_data.mins, return_data.min_id);
        		printf("max=%d from %s\n", return_data.maxs, return_data.max_id);
		}
		for(loop = 0; loop <3; loop++){
			write(client_sockfd[loop],(char *)&return_data,sizeof(return_data));
			close(client_sockfd[loop]);
		}
        }
        close(listen_sockfd);
        return 0;
}
