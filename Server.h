// Server.h

#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>

class Server {
public:
    Server();
    ~Server();
    int run();

private:
    static void *connection_handler(void *sock_fd);
    // Déclarez les autres membres de la classe nécessaires pour le serveur
};

#endif // SERVER_H
