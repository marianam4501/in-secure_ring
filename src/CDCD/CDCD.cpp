#include "CDCD.hpp"

CDCD::CDCD(std::string type, std::string serverIP, std::string clientIP) {
    if (type.compare("s") == 0) {
        this->client = new Client(serverIP, clientIP);
        this->server = NULL;
        this->cryptographer = new Cryptographer();
    }
    if (type.compare("m") == 0) {
        this->client = new Client(serverIP, clientIP);
        this->server = new Server(serverIP);
        this->cryptographer = NULL;
    }
    if (type.compare("r") == 0) {
        this->client = NULL;
        this->server = new Server(serverIP);
        this->cryptographer = new Cryptographer();
    }
    this->type = type;
}

CDCD::~CDCD() {
    if (this->server != NULL) {
        free(this->server);
    }
    if (this->client != NULL) {
        free(this->client);
    }
    if (this->cryptographer != NULL) {
        free(this->cryptographer);
    }
}

bool CDCD::run() {
    bool isRunning = true;
    if (type.compare("s") == 0) {
        isRunning = this->send();
    }
    if (type.compare("m") == 0) {
        isRunning = this->resend();
    }
    if (type.compare("r") == 0) {
        isRunning = this->receive();
    }
    return isRunning;
}

void CDCD::errorLog() {
    this->writeLog("Runtime error: relaunching");
    sleep(1);
}

bool CDCD::send() {
    std::cout << "Send start\n";
    std::string message_count_path = "/home/"+PATH_USER+"/CDCD/000000.txt";
    std::string path = "/home/"+PATH_USER+"/CDCD/";
    const bool stop = false;
    while (!stop) {
        try {
            if (isDesiredTime()) {
                std::string last_msg_processed = FileManager::Read(message_count_path);
                std::string message = "";
                message = FileManager::Read(path+last_msg_processed+".txt");
                if (!message.empty()) {
                    std::cout<<"Sending "<<last_msg_processed<<".txt..."<<std::endl;
                    this->writeLog("Sending message");
                    this->writeStatusFile("Sending message: " + message + "\n");
                    message = this->cryptographer->encrypt(message,"/home/fabian.gonzalezrojas/public_key.pem"); 
                    if (this->client->send(message) == 0) {
                        this->writeLog("Message sent");
                        this->writeStatusFile("Message sent.\n");
                        std::cout << "Sent: [" << message << "]\nLength: [" << message.length() << "]\n";
                        int file_count = std::stoi(last_msg_processed);
                        file_count++;
                        last_msg_processed = generator.convertToZeroPaddedString(file_count);
                        FileManager::Write(last_msg_processed, message_count_path);
                        std::this_thread::sleep_for(std::chrono::minutes(1));
                    }
                }
            } 
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            std::cerr << "\t\tsendCDCD error" << std::endl;
            return false;
        }
        sleep(1);
    }
    std::cout << "Send End\n";
    return false;
}

bool CDCD::resend() {
    std::cout << "Resend start\n";
    const bool stop = false;
    while (!stop) {
        try {
            std::vector<unsigned char> message = this->server->start();
            std::string received_message(message.begin(), message.end());
            this->client->send(received_message);
            std::cout << "Resend: [" << received_message << "]\n";
            this->writeLog("Message received and resended to next computer");
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            std::cerr << "\t\tresendCDCD error" << std::endl;
            return false;
        }
        sleep(1);
    }
    std::cout << "Resend End\n";
    return false;
}

bool CDCD::receive() {
    std::cout << "Receive start\n";
    const bool stop = false;
    while (!stop) {
        try {
            std::vector<unsigned char> message = this->server->start();
            std::string received_message(message.begin(), message.end());
            std::cout << "Length received: [" << received_message.length() << "]\n";
            std::string decrypted_message = this->cryptographer->decrypt(message,"/home/fabian.gonzalezrojas/private_key.pem");
            this->writeLog("Message received");
            this->writeStatusFile("Message received: "+decrypted_message+"\n");
            generator.createMessage(decrypted_message);
            this->writeLog("Message stored");
            this->writeStatusFile("Message stored\n");
            std::cout << "Received: [" << decrypted_message << "]\n";
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            std::cerr << "\t\treceiveCDCD error" << std::endl;
            return false;
        }
        sleep(1);
    }
    std::cout << "Receive End\n";
    return false;
}

void CDCD::writeLog(const std::string message) {
    openlog("Program G4 [CDCD] ", LOG_PID, LOG_LOCAL4);
    syslog(LOG_NOTICE, "%s", message.c_str());
    closelog();
}

void CDCD::writeStatusFile(const std::string message){
    std::string statusFilePath = "/home/"+PATH_USER+"/CDCD/estado.txt";
    std::time_t now = std::time(nullptr);
    std::string currentTime = std::ctime(&now);
    size_t pos = currentTime.find('\n');
    if (pos != std::string::npos) {
        // Eliminar el carácter de salto de línea
        currentTime.erase(pos);
    }
    std::string finalMessage = "Program G4 [CDCD] " +currentTime +": "+ message;
    FileManager::WriteAppend(finalMessage,statusFilePath);
}

std::string CDCD::formatTime(int hour, int minute) {
    std::string formattedHour = hour < 10 ? "0" + std::to_string(hour) : std::to_string(hour);
    std::string formattedMinute = minute < 10 ? "0" + std::to_string(minute) : std::to_string(minute);
    return formattedHour + ":" + formattedMinute;
}

bool CDCD::isDesiredTime() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* currentDateTime = std::localtime(&currentTime);
    int currentHour = currentDateTime->tm_hour;
    int currentMinute = currentDateTime->tm_min;
    const std::vector<std::pair<int, int>> desiredTimes = {
        {0, 40}, {2, 40}, {4, 40}, {6, 40}, {8, 40}, {10, 40},
        {12, 40}, {14, 40}, {16, 40}, {18, 40}, {20, 40}, {22, 40}
    };
    auto desiredTime = std::find(desiredTimes.begin(), desiredTimes.end(), std::make_pair(currentHour, currentMinute));
    if (desiredTime != desiredTimes.end()) {
        std::string currentTimeStr = formatTime(currentHour, currentMinute);
        std::cout << "Current time is: " << currentTimeStr << std::endl;
        return true;
    }
    return false;
}
