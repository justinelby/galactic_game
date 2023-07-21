// Server.h

#ifndef SERVER_H
#define SERVER_H

class Controller; // Forward declaration of Controller class

class Server {
public:
    Server(Controller* controller);
    ~Server();
    int run();

    // Static member function for connection handler
    static void* connection_handler(void* data);

private:
    Controller* controller;
};

#endif // SERVER_H
