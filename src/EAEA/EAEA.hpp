#ifndef EAEA_HPP
#define EAEA_HPP

#include <syslog.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstring>
#include "Server.hpp"
#include "Client.hpp"
#include "FileManager.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>

class EAEA {
  private:
    const std::string PATH_USER =  "fabian.gonzalezrojas";
    Server *server;
    Client *client;
    std::string type;
    FileManager fileManager;
    std::vector<std::string> usernames = {"manuel.arroyo", "rodrigo.piedra", "fabian.gonzalezrojas", 
        "cesar.lopezurena", "nayeri.azofeifa", "leonel.campos", "angie.castillocampos", "mariana.murilloquintana", 
        "jeremy.vargasartavia", "valery.murcia"};

    const std::string extractPubKeyPt1 = "openssl x509 -pubkey -noout -in ";
    const std::string extractPubKeyPt2 = " > /home/"+PATH_USER+"/ca/private/pubkey.pem";
    const std::string verifyCommandPt1 = "bash -c \"echo -n '";
    const std::string verifyCommandPt2 = "' | openssl dgst -sha256 -verify <(openssl rsa -pubin -inform PEM -in /home/"+PATH_USER+"/ca/private/pubkey.pem) -signature /home/"+PATH_USER+"/ca/private/firma.sha256\"";

  public:
    EAEA(std::string type, std::string serverIP, std::string clientIP);

    ~EAEA();

    bool run();

    void errorLog();

  private:
    bool send();

    bool resend();

    bool receive();

    std::vector<std::string> split(const std::string& input, char delimiter);

    void writeLog(const std::string message);

    std::string getCommandOutput(const std::string& command);

    std::string verifySignature(std::string message);

    bool validUsername(std::string username);

    void writeStatusFile(const std::string message, std::string statusFilePath);
};

#endif