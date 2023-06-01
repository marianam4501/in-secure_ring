CC = g++
#FLAGS = -Wall -Wextra -g -std=c++11 # No warnings por ahora
CFLAGS = -g -std=c++17
mainCDCD = -o build/mainCDCD src/CDCD/main.cpp -lcrypto -lssl # -l syslog # -pthread
mainEAEA = -o build/mainEAEA src/EAEA/main.cpp -lcrypto -lssl # -l syslog # -pthread
test = -o build/cryptographer_test src/cryptographer_test.cpp -lcrypto -lssl # -l syslog

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

messageGeneratorCDCD:
		mkdir -p build && \
		$(CC) $(CFLAGS) src/messageGeneratorTest.cpp -o build/message_generatorCDCD

generatorCDCD:
		./build/message_generatorCDCD

testFileManagerEAEA:
		mkdir -p build && \
		$(CC) $(CFLAGS) src/EAEA/testFileManager.cpp -o build/test_fileManagerEAEA

messageGeneratorEAEA:
		mkdir -p build && \
		$(CC) $(CFLAGS) -lcrypto -lssl src/EAEA/MessageGenerator.cpp -o build/message_generatorEAEA

generatorEAEA:
		./build/message_generatorEAEA

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

1: 
		./build/mainCDCD s Argumento_innecesario 192.168.5.42

2: 
		./build/mainCDCD m 192.168.5.42 192.168.5.43

3: 
		./build/mainCDCD m 192.168.5.43 192.168.5.44

4: 
		./build/mainCDCD m 192.168.5.44 192.168.5.45

5: 
		./build/mainCDCD r 192.168.5.45 Argumento_innecesario