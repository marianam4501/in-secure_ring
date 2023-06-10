CC = g++
#FLAGS = -Wall -Wextra -g -std=c++11 # No warnings por ahora
CFLAGS = -g -std=c++17

CDCD = -o build/CDCD/CDCD src/CDCD/CDCD.cpp
ServerCDCD = -o build/CDCD/ServerCDCD src/CDCD/Server.cpp
ClientCDCD = -o build/CDCD/ClientCDCD src/CDCD/Client.cpp
CryptographerCDCD = -o build/CDCD/CryptographerCDCD src/CDCD/Cryptographer.cpp
MessageGeneratorCDCD = -o build/CDCD/MessageGeneratorCDCD src/CDCD/MessageGenerator.cpp
mainCDCD = -o build/CDCD/mainCDCD src/CDCD/main.cpp -lcrypto -lssl # -l syslog # -pthread

mainEAEA = -o build/EAEA/mainEAEA src/EAEA/main.cpp -lcrypto -lssl # -l syslog # -pthread
test = -o build/cryptographer_test src/cryptographer_test.cpp -lcrypto -lssl # -l syslog

cdcd:
		mkdir -p build && mkdir -p build/CDCD && \
		$(CC) $(CFLAGS) $(CDCD) $(ServerCDCD) $(ClientCDCD) $(CryptographerCDCD) $(MessageGeneratorCDCD) $(mainCDCD)

eaea:
		mkdir -p build && \
		$(CC) $(CFLAGS) $(mainEAEA)

test:
		mkdir -p build && \
		$(CC) $(CFLAGS) $(test)

runtest:
		./build/cryptographer_test

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

clean:
		$(RM) build/* && rmdir build && clear

1: 
		./build/CDCD/mainCDCD m 192.168.5.41 192.168.5.42

2: 
		./build/CDCD/mainCDCD m 192.168.5.42 192.168.5.43

3: 
		./build/CDCD/mainCDCD r 192.168.5.43 Argumento_innecesario

4: 
		./build/CDCD/mainCDCD s 192.168.5.44 192.168.5.45

5: 
		./build/CDCD/mainCDCD m 192.168.5.45 192.168.5.41

11: 
		./build/EAEA/mainEAEA m 192.168.5.41 192.168.5.42

22: 
		./build/EAEA/mainEAEA m 192.168.5.42 192.168.5.43

33: 
		./build/EAEA/mainEAEA r 192.168.5.43 Argumento_innecesario

44: 
		./build/EAEA/mainEAEA s 192.168.5.44 192.168.5.45

55: 
		./build/EAEA/mainEAEA m 192.168.5.45 192.168.5.41