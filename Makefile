CC = g++
CFLAGS = -Wall -g -std=c++11

all:
		$(CC) $(CFLAGS) -o build/server src/server.hpp && $(CC) $(CFLAGS) -o build/server src/server.hpp && $(CC) $(CFLAGS) -o build/mains src/main.cpp && $(CC) $(CFLAGS) -o build/mainc src/mainc.cpp

server: 
		$(CC) $(CFLAGS) -o build/server src/server.hpp

client: 
		$(CC) $(CFLAGS) -o build/client src/client.hpp

mains: 
		$(CC) $(CFLAGS) -o build/mains src/main.cpp

mainc: 
		$(CC) $(CFLAGS) -o build/mainc src/mainc.cpp

s: 
		./build/mains

c: 
		./build/mainc 127.0.0.1

clean:
		$(RM) build/*