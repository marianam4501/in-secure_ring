CC = g++
CFLAGS = -Wall -g -std=c++11

server: 
		$(CC) $(CFLAGS) -o server Server.cpp

client: 
		$(CC) $(CFLAGS) -o main main.cpp

clean:
		$(RM) main server client