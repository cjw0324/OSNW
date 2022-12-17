#include <sys/socket.h>  /* 소켓 관련 함수 */
#include <arpa/inet.h>   /* 소켓 지원을 위한 각종 함수 */
#include <sys/stat.h>
#include <stdio.h>      /* 표준 입출력 관련 */
#include <string.h>     /* 문자열 관련 */
#include <unistd.h>     /* 각종 시스템 함수 */
#include <time.h>

#define MAXLINE    1024

char* print_time(struct tm* t);

struct data {
	char str[MAXLINE];
//	int num;
	struct tm* t;
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

    printf("input string : ");
    scanf("%[^\n]s", buf.str);	

//    printf("input integer : ");
//    scanf("%d", &buf.num);

//    buf.num = htonl(buf.num);

    if (write(server_sockfd, &buf.str, sizeof(buf.str)) <= 0) /* 입력 받은 데이터를 서버로 전송한다. */
    {
	    perror("write error : ");
	    return 1;
    }

    while(1) {
	    memset(&buf, 0x00, sizeof(buf));
	    /* 서버로 부터 데이터를 읽는다. */
	    if (read(server_sockfd, &buf, sizeof(buf)) <= 0)
	    {
       		 perror("read error : ");
		 return 1;
	    }
	    printf("read\n");
	    sleep(1);
//	    buf.num = ntohl(buf.num);
	    printf("%s\n", print_time(buf.t));
	    printf("read : %s\n", buf.str);
    }

    close(server_sockfd);
    return 0;
}

char* print_time(struct tm* t)
{
        char week[4];
        char month[4];
        char str_mday[3];
        char str_hour[3];
        char str_min[3];
        char str_sec[3];
        char str_year[5];
        static char time[1024];

        switch(t->tm_mon+1){
                case 1:
                        strcpy(month, "JAN");
                        break;
                case 2:
                        strcpy(month, "FEB");
                        break;

                case 3:
                        strcpy(month, "MAR");
                        break;

                case 4:
                        strcpy(month, "APR");
                        break;

                case 5:
                        strcpy(month, "MAY");
                        break;

                case 6:
                        strcpy(month, "JUN");
                        break;

                case 7:
                        strcpy(month, "JUL");
                        break;

                case 8:
                        strcpy(month, "AUG");
                        break;

                case 9:
                        strcpy(month, "SEP");
                        break;

                case 10:
                        strcpy(month, "OCT");
                        break;

                case 11:
                        strcpy(month, "NOV");
                        break;

                case 12:
                        strcpy(month, "DEC");
                        break;
        }
        switch(t->tm_wday){
                case 0:
                        strcpy(week, "SUN");
                        break;
                case 1:
                        strcpy(week, "MON");
                        break;
                case 2:
                        strcpy(week, "TUE");
                        break;

                case 3:
                        strcpy(week, "WED");
                        break;

                case 4:
                        strcpy(week, "THU");
                        break;

                case 5:
                        strcpy(week, "FRI");
                        break;

                case 6:
                        strcpy(week, "SAT");
                        break;
        }
        sprintf(time,"%s %s %d %d:%d:%d %d\n",week,month,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,t->tm_year+1900);
        printf("%s\n", time);
        return time;
}
