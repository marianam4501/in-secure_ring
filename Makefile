CC = g++
#FLAGS = -Wall -Wextra -g -std=c++11 # No warnings por ahora
CFLAGS = -g -std=c++11
mainCDCD = -o build/mainCDCD src/CDCD/main.cpp -pthread
mainEAEA = -o build/mainEAEA src/EAEA/main.cpp -pthread

cdcd:
		mkdir -p build && \
		$(CC) $(CFLAGS) $(mainCDCD)

eaea:
		mkdir -p build && \
		$(CC) $(CFLAGS) $(mainEAEA)

s1: 
		./build/mainCDCD s 127.0.0.1

m1: 
		./build/mainCDCD m 127.0.0.1

r1: 
		./build/mainCDCD r 127.0.0.1

s2: 
		./build/mainEAEA s 127.0.0.1

m2: 
		./build/mainEAEA m 127.0.0.1

r2: 
		./build/mainEAEA r 127.0.0.1

clean:
		$(RM) build/* && rmdir build && clear
