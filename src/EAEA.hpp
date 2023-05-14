#ifndef EAEA_HPP
#define EAEA_HPP

#include <iostream>

#include "Server.hpp"
#include "Client.hpp"

class EAEA {
  private:
    Server *server;
    Client *client;

  public:
    EAEA() {
      this->server = new Server();
      this->client = new Client();
    }

    ~EAEA() {
      free(this->server);
      free(this->client);
    }

    /**
     * Este canal debería encargarse de todas las comprobaciones y demás cosas antes 
     * de enviar el mensaje. Hay que implementar métodos de autenticación y cifrado
    */
    void sendEAEA(const char* server_address, const char* message) {
        /* De momento me parece que es apropiado que solo envie el mensaje sin ningún tipo
        de cifrado o autenticación para poder montar el diagrama de interacción  */
        try {
            this->client->connect(server_address);
            client->send(message);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void receiveEAEA(const char* server_address) {
        /* De momento me parece que es apropiado que solo reciba el mensaje sin ningún tipo
        de cifrado o autenticación para poder montar el diagrama de interacción  */
        try {
            Server server;
            char* message = server.start();
            printf("[EAEA]: Recibido {%s}", message);
            free(message);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};

#endif