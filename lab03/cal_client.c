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
#define IP "127.0.0.1"
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
    	struct sockaddr_in serveraddr;
    	int server_sockfd;
    	int len;
    	int sbyte, rbyte;
    	struct cal_data sdata;
	struct result_data return_data;
    	if (argc != 4)
   	{
		printf("Usage : %s [num1] [num2] [op]\n", argv[0]);
   		return 1;
    	}
    	memset((void *)&sdata, 0x00, sizeof(sdata));
	sdata.left_num = atoi(argv[1]);
    	sdata.right_num = atoi(argv[2]);
    	sdata.op = argv[3][0];

    	/* TCP 기반 통신의 소켓 생성 */
        if ((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
        {
                perror("error :");
                return 1;
        }

    	serveraddr.sin_family = AF_INET;
    	serveraddr.sin_port = htons(PORT);
    	serveraddr.sin_addr.s_addr = inet_addr(IP);

    	if (connect(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1 )
    	{
   		printf("fail to connect\n");
   	 	close(server_sockfd);
   	 	return 1;
    	}

    	len = sizeof(sdata);
    	sdata.left_num = htonl(sdata.left_num);
    	sdata.right_num = htonl(sdata.right_num);
    	write(server_sockfd, (char *)&sdata, len);

        if (ntohs(sdata.error != 0))
        {
                printf("CALC Error %d\n", ntohs(sdata.error));
        }
	/* 서버로 부터 데이터를 읽는다. */
        read(server_sockfd,(char*)&return_data,sizeof(return_data));
	printf("min=%d from %s\n", return_data.mins, return_data.min_id);
	printf("max=%d from %s\n", return_data.maxs, return_data.max_id);
	close(server_sockfd);
    	return 0;
}
