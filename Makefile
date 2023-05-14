CC = g++
#FLAGS = -Wall -Wextra -g -std=c++11 # No warnings por ahora
CFLAGS = -g -std=c++11
Server = -o build/Server src/Server.hpp
Client = -o build/Client src/Client.hpp
CDCD = -o build/CDCD src/Server.hpp
EAEA = -o build/EAEA src/EAEA.hpp
RingComputer = -o build/RingComputer src/RingComputer.hpp
RingComputerSender = -o build/RingComputerSender src/RingComputerSender.hpp
RingComputerMiddle = -o build/RingComputerMiddle src/RingComputerMiddle.hpp
RingComputerReceiver = -o build/RingComputerReceiver src/RingComputerReceiver.hpp
main = -o build/main src/main.cpp -pthread

all:
		mkdir -p build && \
		$(CC) $(CFLAGS) $(main)

main: 
		$(CC) $(CFLAGS) $(main)

run: 
		./build/main

args:
		./build/main 127.0.0.1

clean:
		$(RM) build/* && rmdir build && clear

# Al parecer no hace falta compilar todo, solo el main
#		$(CC) $(CFLAGS) $(Server) && \
#		$(CC) $(CFLAGS) $(Client) && \
#		$(CC) $(CFLAGS) $(CDCD) && \
#		$(CC) $(CFLAGS) $(EAEA) && \
#		$(CC) $(CFLAGS) $(CDCD) && \
#		$(CC) $(CFLAGS) $(RingComputer) && \
#		$(CC) $(CFLAGS) $(RingComputerSender) && \
#		$(CC) $(CFLAGS) $(RingComputerMiddle) && \
#		$(CC) $(CFLAGS) $(RingComputerReceiver) && \
