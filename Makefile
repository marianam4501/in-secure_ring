CC = g++
#FLAGS = -Wall -Wextra -g -std=c++11 # No warnings por ahora
CFLAGS = -g -std=c++11
Server = -o build/Server src/Server.hpp
Client = -o build/Client src/Client.hpp
CDCD = -o build/CDCD src/Server.hpp
EAEA = -o build/EAEA src/EAEA.hpp
RingComputer = -o build/RingComputer src/RingComputer.hpp
RingComputerSender = -o build/RingComputerSender src/RingComputerSender.hpp

all:
		mkdir -p build && \
		$(CC) $(CFLAGS) $(Server) && \
		$(CC) $(CFLAGS) $(Client) && \
		$(CC) $(CFLAGS) $(CDCD) && \
		$(CC) $(CFLAGS) $(EAEA) && \
		$(CC) $(CFLAGS) $(CDCD) && \
		$(CC) $(CFLAGS) $(RingComputer) && \
		$(CC) $(CFLAGS) -o build/main src/main.cpp -pthread

main: 
		$(CC) $(CFLAGS) -o build/main src/main.cpp -pthread

run: 
		./build/main

args:
		./build/main 127.0.0.1

clean:
		$(RM) build/* && rmdir build && clear