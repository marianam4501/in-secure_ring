CC = g++
CFLAGS = -Wall -g -std=c++11

server: 
		$(CC) $(CFLAGS) -o server server.hpp

client: 
		$(CC) $(CFLAGS) -o client client.hpp

1: 
		./client

2: 
		./server

clean:
		$(RM) main server client
