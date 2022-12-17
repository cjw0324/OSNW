#include <sys/socket.h>  /* 소켓 관련 함수 */
#include <arpa/inet.h>   /* 소켓 지원을 위한 각종 함수 */
#include <sys/stat.h>
#include <stdio.h>      /* 표준 입출력 관련 */
#include <string.h>     /* 문자열 관련 */
#include <unistd.h>     /* 각종 시스템 함수 */
#include <time.h>

#define MAXLINE    1024

struct data {
	char str[MAXLINE];
	struct tm* num;
};

int main(int argc, char **argv)
{
    struct sockaddr_in serveraddr;
    int server_sockfd;
    int client_len;
    struct data buf;

    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("error :");
        return 1;
    }

    /* 연결요청할 서버의 주소와 포트번호 프로토콜등을 지정한다. */
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(3600);

    client_len = sizeof(serveraddr);

    /* 서버에 연결을 시도한다. */
    if (connect(server_sockfd, (struct sockaddr *)&serveraddr, client_len)  == -1)
    {
        perror("connect error :");
        return 1;
    }

    memset(&buf, 0x00, sizeof(buf));

    strcpy(buf.str,argv[1]);
    time_t t;
    t = time(NULL);
    buf.num = localtime(&t);
    if (write(server_sockfd, &buf, sizeof(buf)) <= 0) /* 입력 받은 데이터를 서버로 전송한다. */
    {
	    perror("write error : ");
	    return 1;
    }

    while(1) {
	    memset(&buf, 0x00, sizeof(buf));
	    /* 서버로 부터 데이터를 읽는다. */
	    if (read(server_sockfd, &buf.str, sizeof(buf.str)) <= 0)
	    {
       		 perror("read error : ");
		 return 1;
	    }
	    read(server_sockfd, &buf.num, sizeof(buf.num));
	    time_t t;
    	    t = time(NULL);
            buf.num = localtime(&t);
	    printf("%s ", buf.str);
	    printf("%s", asctime(buf.num));
    }

    close(server_sockfd);
    return 0;
}


