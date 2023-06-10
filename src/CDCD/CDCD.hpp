#ifndef CDCD_HPP
#define CDCD_HPP

#include <syslog.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include "Server.hpp"
#include "Client.hpp"
#include "Cryptographer.hpp"
#include "FileManager.hpp"
#include "MessageGenerator.hpp"
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>

class CDCD {
  private:
    Server *server;
    Client *client;
    Cryptographer *cryptographer;
    std::string type;
    MessageGenerator generator;
    const std::string PATH_USER =  "fabian.gonzalezrojas";

  public:
    CDCD(std::string type, std::string serverIP, std::string clientIP);

    ~CDCD();

    bool run();

    void errorLog();

    bool send();

    bool resend();

    bool receive();

  private:
    void writeLog(const std::string message);

    void writeStatusFile(const std::string message);

    std::string formatTime(int hour, int minute);

    bool isDesiredTime();
};

#endif
