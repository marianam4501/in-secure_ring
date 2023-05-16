#ifndef CDCD_HPP
#define CDCD_HPP

#include <iostream>

#include "Server.hpp"
#include "Client.hpp"

class CDCD {
  private:
    Server *server;
    Client *client;

  public:
    CDCD(const bool serverUp, const bool clientUp) {
        if(serverUp) {
            this->server = new Server();
        } else {
            this->server = NULL;
        }
        if(clientUp) {
            this->client = new Client();
        } else {
            this->client = NULL;
        }
    }

    ~CDCD() {
        if (this->server != NULL) {
            free(this->server);
        }
        if (this->client != NULL) {
            free(this->client);
        }
    }

    /**
     * Este canal debería encargarse de todas las comprobaciones y demás cosas antes 
     * de enviar el mensaje. Hay que implementar métodos de autenticación y cifrado
    */
    void sendCDCD(const char* ip_direction) {
        /* De momento me parece que es apropiado que solo envie el mensaje sin ningún tipo
        de cifrado o autenticación para poder montar el diagrama de interacción  */
        try {
            this->client->connect(ip_direction);
            client->send("Hello from client");
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void receiveCDCD(const char* ip_direction) {
        /* De momento me parece que es apropiado que solo reciba el mensaje sin ningún tipo
        de cifrado o autenticación para poder montar el diagrama de interacción  */
        try {
            Server server;
            server.start();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

#endif