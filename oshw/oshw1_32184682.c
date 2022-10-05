#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int main(void){
	pid_t pid;  //pid 변수 선언
	
	int status;   //변수 설정
	int x=0;
	int y=0;
	int interval=0;
	int sum = 0;
	scanf("%d %d %d", &x, &y, &interval);  //x, y, z 입력
	pid = fork();  //자식프로세스 생성
	if(pid > 0){           //부모프로세스
		wait(&status); 
		if(WEXITSTATUS(status) == 1 ){  //자식 종료 코드가 1인 경우 1000미만 출력
			printf("Parent says that it is 1000.\n");
			return 0;
		}
		else if(WEXITSTATUS(status) == 2){  //자식 종료 코드가 2인 경우 1000 초과 출력
			printf("Parent says that it is greater than 1000.\n");
			return 0;
		}
		else if(WEXITSTATUS(status) == 0) {  //자식 종료 코드가 0인 경우 1000 동일 출력
			printf("Parent says that it is less than 1000.\n");
			return 0;
		}
		else{                 //자식 종료코드가 3인 경우 부모 프로세스 출력없이 종료
			return 0;
		}
	}
	else if(pid == 0){     //자식프로세스
		if(x<=0 || y<=0 || interval<=0){   //정상 입력 유무 확인
			if(x<=0){
				printf("x is not a positive integer\n");  //x 정상입력 확인 
			}
			if(y<=0){
				printf("y is not a positive integer\n");  //y 정상입력 확인
			}
			if(interval<=0){                                  //z 정상입력 확인
				printf("interval is not a positive integer\n");
			}
			exit(3);      //비정상 입력 시 자식프로세스 종료코드 3부여
		}
		else{
			for(int i=x; i<= y; i+=interval){     //x 부터 y까지 interval만큼 간격두고 더한값 계산
				sum = sum+i;
			}
			printf("Child says that sum of numbers from %d to %d with interval of %d is %d\n", x, y, interval, sum);  //자식프로세스 출력
			if(sum < 1000){
				return 0;      //종료시 반환값 0 부여
			}
			else if(sum == 1000){
				return 1;      //종료시 반환값 1 부여
			}
			else{
				return 2;      //종료시 반환값 2 부여
			}
		}
	}
	else{                  //fork 실패
		printf("자식 프로세스 생성 실패!\n");     //fork 실패 문구 출력
	}

}

