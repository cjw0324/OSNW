#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct tm* get_time(struct tm* t)
{
	time_t timer;
	timer = time(NULL);
	t = localtime(&timer);
	printf("success get_time\n");
}


void print_time(struct tm* t)
{
	printf("income print_time\n");
	char week[4];
	char month[4];
	//char str_mday[3];
	//char str_hour[3];
	//char str_min[3];
	//char str_sec[3];
	//char str_year[5];
	char time[1024];
	//const static char* mon[12] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
	//const static char* day[7] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
	printf("%s", asctime(t));
	//printf("month : %s\n", mon[0]);
	//printf("month : %d\n", t->tm_mon);
	//printf("month : %s\n", mon[t->tm_mon]);
	//strcpy(month,mon[t->tm_mon]);
	//strcpy(week,day[t->tm_wday]);
	//printf("success middle of print_time\n");
	//sprintf(time,"%s %s %d %d:%d:%d %d\n",week,month,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,t->tm_year+1900);
	//printf("%s\n", time);;
}

void main()
{
	struct tm* tt;
	get_time(tt);
	printf("success main get_time\n");
	printf("%s", asctime(tt));
}

