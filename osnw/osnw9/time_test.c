#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct tm* get_time()
{
	time_t timer;
	struct tm* t;
	timer = time(NULL);
	t = localtime(&timer);
	return t;
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
	char time[1024];

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

void main()
{
	char* now;
	now = print_time(get_time());
	printf("main : %s\n", now);
}
