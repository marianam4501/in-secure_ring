CC = g++
#FLAGS = -Wall -Wextra -g -std=c++11 # No warnings por ahora
CFLAGS = -g -std=c++11
mainCDCD = -o build/mainCDCD src/CDCD/main.cpp -lcrypto -lssl # -pthread
mainEAEA = -o build/mainEAEA src/EAEA/main.cpp -lcrypto -lssl # -pthread
test = -o build/cryptographer_test src/cryptographer_test.cpp -lcrypto -lssl

cdcd:
		mkdir -p build && \
		$(CC) $(CFLAGS) $(mainCDCD)

eaea:
		mkdir -p build && \
		$(CC) $(CFLAGS) $(mainEAEA)

test:
		mkdir -p build && \
		$(CC) $(CFLAGS) $(test)

runtest:
		./build/cryptographer_test

logger:
		$(CC) $(CFLAGS) src/log_manager_test.cpp -o build/logger_test

runlogger:
		./build/logger_test

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
