#include <arpa/inet.h>   /* 소켓 지원을 위한 각종 함수 */
#include <sys/stat.h>
#include <stdio.h>      /* 표준 입출력 관련 */
#include <string.h>     /* 문자열 관련 */
#include <unistd.h>     /* 각종 시스템 함수 */
#include <time.h>
#include <stdlib.h>


#define MAXLINE    1024

struct data {
        char str[MAXLINE];
};


int main(int argc, char **argv)
{
    struct sockaddr_in serveraddr;
    int server_sockfd;
    int client_len;
    struct data buf;

    fd_set readfds, allfds;
    int fd_num;
    int maxfd = 0;
    time_t t;
    struct tm *lt = malloc(sizeof(struct tm));

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

    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    FD_SET(server_sockfd, &readfds);

    maxfd = server_sockfd;

    while(1){
	    allfds = readfds;
	    fd_num = select(maxfd+1, &allfds, (fd_set *)0, (fd_set *)0, NULL);

	    if(FD_ISSET(server_sockfd, &allfds)){
		    memset(&buf, 0x00, sizeof(buf));
		    memset(lt, 0x00, sizeof(struct tm));
		    /* 서버로 부터 데이터를 읽는다. */
		    if(read(server_sockfd, &buf, sizeof(buf)) <= 0)
		    {
			    printf("client string read error\n");
			    perror("read error : ");
			    return 1;
		    }
		    if(read(server_sockfd, lt, sizeof(struct tm)) <= 0)
                    {
			    printf("client time read error\n");
                            perror("read error : ");
                            return 1;
                    }
		    printf("%s %s", buf.str, asctime(lt));
	    }

	    if(FD_ISSET(0, &allfds)){
		    memset(&buf, 0x00, sizeof(buf));
		    memset(lt, 0x00, sizeof(struct tm));
		    scanf("%s", buf.str);

		    if((t = time(NULL)) == -1) {
        		perror("time() call error");
        		return -1;
    		    }

    	            if((lt = localtime(&t)) == NULL) {
        		perror("localtime() call error");
        		return -1;
    		    }
		    //printf("time 생성 완료\n");
		    if(write(server_sockfd, &buf, sizeof(buf)) <= 0){
			    perror("string write error : ");
			    return 1;
		    }
		    if(write(server_sockfd, lt, sizeof(struct tm)) <= 0){
                            perror("time write error : ");
                            return 1;
                    }
		    memset(lt, 0x00, sizeof(struct tm));
	    }
    }
    close(server_sockfd);
    return 0;
}
