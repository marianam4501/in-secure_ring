CC = g++
FLAGS = -Wall -Wextra
CFLAGS = -g -std=c++17

CDCD = -o build/CDCD/CDCD src/CDCD/CDCD.cpp
ServerCDCD = -o build/CDCD/ServerCDCD src/CDCD/Server.cpp
ClientCDCD = -o build/CDCD/ClientCDCD src/CDCD/Client.cpp
CryptographerCDCD = -o build/CDCD/CryptographerCDCD src/CDCD/Cryptographer.cpp
MessageGeneratorCDCD = -o build/CDCD/MessageGeneratorCDCD src/CDCD/MessageGenerator.cpp
mainCDCD = -o build/CDCD/mainCDCD src/CDCD/main.cpp -lcrypto -lssl # -l syslog # -pthread

EAEA = -o build/EAEA/EAEA src/EAEA/EAEA.cpp
ServerEAEA = -o build/EAEA/ServerEAEA src/EAEA/Server.cpp
ClientEAEA = -o build/EAEA/ClientEAEA src/EAEA/Client.cpp
FileManagerEAEA = -o build/EAEA/FileManagerEAEA src/EAEA/FileManager.cpp
mainEAEA = -o build/EAEA/mainEAEA src/EAEA/main.cpp -lcrypto -lssl # -l syslog # -pthread

cdcd:
		mkdir -p build && mkdir -p build/CDCD && \
		$(CC) $(FLAGS) $(CFLAGS) $(CDCD) $(ServerCDCD) $(ClientCDCD) $(CryptographerCDCD) $(MessageGeneratorCDCD) $(mainCDCD)

eaea:
		mkdir -p build && mkdir -p build/EAEA && \
		$(CC) $(FLAGS) $(CFLAGS) $(EAEA) $(ServerEAEA) $(ClientEAEA) $(FileManagerEAEA)  $(mainEAEA)

msgEAEA:
		mkdir -p build && \
		$(CC) $(CFLAGS) $(FileManagerEAEA) -lcrypto -lssl src/EAEA/MessageGenerator.cpp -o build/message_generatorEAEA

msg:
		./build/message_generatorEAEA

clean:
		$(RM) build/CDCD/* && $(RM) build/EAEA/* && rmdir build/* && rmdir build && clear

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