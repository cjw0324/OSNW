TARGET=client server
OBJS=client.o server

all : client server

client : client.c
	gcc -o client client.c
echo_server : server.c
	gcc -o server server.c
clean:
	rm -f client server

