all : echo_server echo_client

echo_server : echo_server.c
	gcc -o echo_server echo_server.c
echo_client : echo_client.c
	gcc -o echo_client echo_client.c
clean:
	rm -f echo_client echo_server
