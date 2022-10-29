TARGET=cal_client cal_server
OBJS=cal_client.o cal_server.o

all : cal_client cal_server

cal_client : cal_client.c
	gcc -o cal_client cal_client.c
cal_server : cal_server.c
	gcc -o cal_server cal_server.c
clean:
	rm -f cal_client cal_server 
