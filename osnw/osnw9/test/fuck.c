#include <stdio.h>
#include <time.h>

void main()
{
	time_t t;
	struct tm* now;
	t = time(NULL);
	now = localtime(&t);
	printf("%s", asctime(now));
	printf("%ld\n", sizeof(now));
	int* num;
	int p;
	num = now;
	p = &now;
	printf("%ld\n", num);
	struct tm* time;
	time = num;
	printf("%ld\n", time);
	printf("%s", asctime(time));
	printf("%d\n", p);
}
