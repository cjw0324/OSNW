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

uint8_t key[16] = {0, };
struct dirent *ent;
DIR *dir;
FILE *fp;

int filesize(const char *filename){

    	struct stat file_info;
	int sz_file;
	if ( 0 > stat( filename, &file_info)){
		return -1; // file이 없거나 에러
	}
	return file_info.st_size;
}


void *attack(void *param) { //1번 스레드 실행 함수
	printf("attack thread\n");
	printf("%s\n", key);
	if((dir = opendir ("./target")) != NULL){
		while((ent = readdir(dir)) != NULL){
			//printf("%s\n", ent->d_name);
			if((strcmp(ent->d_name, ".") == 0)||(strcmp(ent->d_name, "..") == 0)){
				continue;
			}
			char name [50] = "./target/";
			strcat(name, ent->d_name);
			//printf("%s\n", name);
			printf("[%s]attack before size : %dbite\n", ent->d_name, filesize(name));
			fp = fopen(name, "a");
			fseek(fp,1, SEEK_SET);
			fwrite(key, 1, sizeof(key),fp);
			fclose(fp);
			printf("[%s]attack aftere size :%dbite\n", ent->d_name, filesize(name));
		}
		closedir (dir);
	} else {
		return EXIT_FAILURE;
	}
	pthread_exit(NULL); //1번 스레드 종료
}

void *restore(void *param) { //2번 스레드 실행 함수
	printf("restore thread\n");
	if((dir = opendir ("./target")) != NULL){
                while((ent = readdir(dir)) != NULL){
                        printf("%s\n", ent->d_name);
                        if((strcmp(ent->d_name, ".") == 0)||(strcmp(ent->d_name, "..") == 0)){
                                continue;
                        }
                        char name [50] = "./target/";
                        strcat(name, ent->d_name);
                        printf("%s\n", name);
                        printf("%d\n", filesize(name));
                        fp = fopen(name, "a+");
                        fwrite(key, 1, sizeof(key),fp);
                        fclose(fp);
                        printf("%d\n", filesize(name));
                }
                closedir (dir);
        } else {
                return EXIT_FAILURE;
        }
	pthread_exit(NULL); //2번 스레드 종료
}

int main(int argc, char*argv[]) {
	char* attackk = "attack";
	char* restoree = "restore";
	pthread_t f;

	if(argc != 3)
	{
		printf("error 1\n");
		exit(1);
	}
	if((strcmp(argv[1], attackk) != 0) && (strcmp(argv[1], restoree) != 0) )
	{
		//printf("strcmp(argv[1], attack) : %d, strcmp(argv[1], restore) : %d\n", strcmp(argv[1], attackk), strcmp(argv[1], restoree));
		exit(2);
	}
	printf("key : \t");
	for (int i = 0; i < 16; i++) {
		key[i] = argv[2][i];
		printf("%c", argv[2][i]);
	}
	printf("\n");
	/*
	for( int i = 0; i < 16; i++) {
		printf("%d", key[i]);
	}
	printf("\n");
	printf("bite size : %ld\n", sizeof(key));
	*/
	//test

	if(strcmp(argv[1], attackk) == 0)
	{
		printf("attack 실행\n");
		if(pthread_create(&f, NULL, attack, NULL) != 0) //1번 thread 생성
                {
			fprintf(stderr, "thread create error\n");
                        exit(1);
                }
	}
	else
	{
		printf("restore 실행\n");
        	if(pthread_create(&f, NULL, restore, NULL) != 0)  //2번 thread 생성
               	{
			fprintf(stderr, "thread create error\n");
                        exit(1);
                }
	}

	pthread_join(f, NULL); //스레드 자원 회수

	return 0;
}

