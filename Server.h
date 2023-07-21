// Server.h

#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "./include/rapidjson/document.h"
#include "./include/rapidjson/writer.h"
#include "./include/rapidjson/stringbuffer.h"

class Controller; // Forward declaration of the Controller class

class Server
{
public:
    // Constructor
    Server(Controller* controller);

    // Destructor
    ~Server();

    // Method to run the server
    int run();

    // Method to handle the request from the client
    void handleRequest(const rapidjson::Document& document, rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

    // Method to handle the "GetCharacterInfo" request
    void handleGetCharacterInfo(const rapidjson::Value& getCharacterInfo, rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

    // Method to handle the "GetAllCharacters" request
    void handleGetAllCharacters(const rapidjson::Value& getAllCharacters, rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

    // Method to handle the "GetSpaceshipInfo" request
    void handleGetSpaceshipInfo(const rapidjson::Value& getSpaceshipByName, rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

    // Method to handle the "GetAllSpaceships" request
    void handleGetAllSpaceships(const rapidjson::Value& getAllSpaceships, rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

    // Method to handle the "GetPlanetInfo" request
    void handleGetPlanetInfo(const rapidjson::Value& getPlanetByName, rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

    // Method to handle the "GetAllPlanets" request
    void handleGetAllPlanets(const rapidjson::Value& getAllPlanets, rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

    // Method to handle the "GetQuestInfo" request
    void handleGetQuestInfo(const rapidjson::Value& getQuestByName, rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

    // Method to handle the "GetAllQuests" request
    void handleGetAllQuests(const rapidjson::Value& getAllQuests, rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

    // Method to handle the "addCharacter" request
    void handleAddCharacter(const rapidjson::Value& addCharacterData) const;

    // ... Other methods and members of the Server class ...

private:
    // Static method to handle the connection with the client in a separate thread
    static void* connection_handler(void* data);

    // Pointer to the Controller instance
    Controller* controller;
};

#endif // SERVER_H
