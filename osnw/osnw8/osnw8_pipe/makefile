TARGET=echo_client echo_server_fork
OBJS=echo_client.o echo_server_fork

all : echo_client echo_server_fork

echo_client : echo_client.c
	gcc -o echo_client echo_client.c
echo_server : echo_server.c
	gcc -o echo_server_fork echo_server_fork.c
clean:
	rm -f echo_client echo_server_fork

