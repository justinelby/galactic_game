// Server.cpp

#include "Server.h"
#include "Controller.h"
#include <cstring> // pour la fonction strerror
#include <netinet/in.h>
#include <iostream> // input output
#include <fstream>  // file management
#include <string>   // strlen string
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h> // multi threading
#include <errno.h>
#include <signal.h>
#include <iomanip> // pour std::setprecision

// RapidJSON includes
#include "./include/rapidjson/document.h"
#include "./include/rapidjson/writer.h"
#include "./include/rapidjson/stringbuffer.h"

// server address
#define ADDRESS "0.0.0.0"
// port number
#define PORT 8044
// maximum concurrent connections
#define CONCURRENT_CONNECTION 10
// maximum connection requests queued
#define QUEUE_CONNECTION 20
// buffer size 1KB
#define BUFFER_SIZE 1024
// Thread stack size 64KB
#define THREAD_STACK_SIZE 65536
// current connections
int connection = 0;

struct ConnectionData
{
    int conn_id;
    Controller *controller;
};

Server::Server(Controller *controller) : controller(controller)
{
    // Initialize any necessary members
}

Server::~Server()
{
    // Clean up any resources used by the server
}

int Server::run()
{
    // thread identifier
    pthread_t thread_id;
    // thread attribute
    pthread_attr_t attr;

    if (pthread_attr_init(&attr) != 0)
    {
        std::cout << "[ERROR][THREAD][INIT] " << strerror(errno) << "\n";
        return -1;
    }

    // stack size 1MB
    if (pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE) != 0)
    {
        std::cout << "[ERROR][THREAD][STACK] " << strerror(errno) << "\n";
        return -1;
    }

    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
    {
        std::cout << "[ERROR][THREAD][DETACH] " << strerror(errno) << "\n";
        return -1;
    }

    int master_socket, conn_id;
    struct sockaddr_in server, client;

    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));

    signal(SIGPIPE, SIG_IGN);

    // creating master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        std::cout << "[ERROR] CAN'T CREATE TO SOCKET\n";
        return -1;
    }
    else
    {
        std::cout << "[NOTE] SOCKET CREATED DONE\n";
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ADDRESS);
    server.sin_port = htons(PORT);

    socklen_t addrlen = sizeof(struct sockaddr_in);

    // binding address and port
    if (bind(master_socket, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        std::cout << "[ERROR][BIND] " << strerror(errno) << "\n";
        return -1;
    }
    else
    {
        std::cout << "[NOTE] BIND " << ADDRESS << ":" << PORT << "\n";
    }

    // Listen on the socket, with 20 max connection requests queued
    if (listen(master_socket, QUEUE_CONNECTION) == -1)
    {
        std::cout << "[ERROR][LISTEN] " << strerror(errno) << "\n";
        return -1;
    }
    else
    {
        std::cout << "[INFO] WAITING FOR INCOMING CONNECTIONS\n";
    }

    // infinity loop
    while (true)
    {
        // accept new connections
        conn_id = accept(master_socket, (struct sockaddr *)&client, (socklen_t *)&addrlen);

        // if connection acception failed
        if (conn_id == -1)
        {
            std::cout << "[WARNING] CAN'T ACCEPT NEW CONNECTION\n";
        }
        else
        {
            // if connection limit reached
            if (connection >= CONCURRENT_CONNECTION)
            {
                std::cout << "[WARNING] CONNECTION LIMITE REACHED\n";
                // send(conn_id, "server is busy. please try again later.", 39, 0);
                close(conn_id); // close connection
            }
            else
            {
                std::cout << "[INFO] NEW CONNECTION ACCEPTED FROM " << inet_ntoa(client.sin_addr) << ":"
                          << ntohs(client.sin_port) << "\n";
                // create new thread for new connection
                ConnectionData *data = new ConnectionData;
                data->conn_id = conn_id;
                data->controller = controller;
                if (pthread_create(&thread_id, &attr, &Server::connection_handler, data) == -1)
                {
                    std::cout << "[WARNING] CAN'T CREATE NEW THREAD\n";
                    close(conn_id);
                    delete data;
                }
                else
                {
                    std::cout << "[INFO] NEW THREAD CREATED\n";
                    connection++; // increase connection count
                }
            }
        }
    }
    return 0;
}

// This will handle connection for each client
void *Server::connection_handler(void *data)
{
    /* clock_t clock(void) returns the number of clock ticks
       elapsed since the program was launched.To get the number
       of seconds used by the CPU, you will need to divide by
       CLOCKS_PER_SEC.where CLOCKS_PER_SEC is 1000000 on typical
       32 bit system.  */
    clock_t start, end;

    // Recording the starting clock tick.
    start = clock();

    // byte size
    int read_byte = 0;

    // Get the socket descriptor
    int conn_id = reinterpret_cast<ConnectionData *>(data)->conn_id;
    Controller *controller = reinterpret_cast<ConnectionData *>(data)->controller;

    // request data
    char buffer[BUFFER_SIZE] = {0};

    // read response continue
    std::string receivedData;

    while ((read_byte = recv(conn_id, buffer, BUFFER_SIZE, 0)) > 0)
    {
        receivedData += buffer;

        std::cout << "[RECEIVED] " << buffer << "\n";
        // clear buffer data
        memset(buffer, 0, BUFFER_SIZE);

        // Enregistrer le contenu JSON dans le fichier
        // Rechercher la position du premier '{' dans receivedData
        size_t startPos = receivedData.find_first_of('{');

        // Rechercher la position du dernier '}' dans receivedData
        size_t endPos = receivedData.find_last_of('}');

        if (startPos != std::string::npos && endPos != std::string::npos)
        {
            // Extraire la partie JSON de receivedData
            std::string jsonContent = receivedData.substr(startPos, endPos - startPos + 1);

            // Enregistrer le contenu JSON dans le fichier
            std::ofstream outputFile("./cmake-build-debug/actionsData.json");
            if (outputFile.is_open())
            {
                outputFile << jsonContent;
                outputFile.close();
                std::cout << "Message JSON enregistré dans le fichier actionsData.json\n";
            }
            else
            {
                std::cout << "Erreur lors de l'ouverture du fichier actionsData.json pour l'enregistrement du message JSON\n";
            }

            std::cout << "[RECEIVED JSON]: " << jsonContent << "\n";

            receivedData.clear(); // Effacer le contenu traité
        }
        else
        {
            std::cout << "Erreur : Aucun objet JSON trouvé dans les données reçues\n";
        }

        std::cout << "[RECEIVED JSON]: " << receivedData << "\n";

        std::ifstream file("./cmake-build-debug/actionsData.json");
        if (!file.is_open())
        {
            std::cout << "Le fichier de chargement des fonctions ne s'est pas ouvert" << std::endl;
        }
        // Lecture du contenu du fichier JSON
        std::string fileJsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Création du document JSON
        rapidjson::Document document;
        document.Parse(fileJsonContent.c_str());

        // Find the method name from the JSON request
        string methodName;
        for (auto &member : document.GetObject())
        {
            if (member.name.IsString())
            {
                methodName = member.name.GetString();
                break;
            }
        }

        // Prepare the JSON response
        rapidjson::StringBuffer sb;
        rapidjson::Writer<rapidjson::StringBuffer> writer(sb);

        // GetCharacterInfo Function
        if (methodName == "GetCharacterInfo")
        {
            const rapidjson::Value &getCharacterInfo = document["GetCharacterInfo"];

            writer.StartObject();
            writer.Key("GetCharacterInfo");
            writer.StartObject();
            if (getCharacterInfo.HasMember("CharacterName"))
            {
                string characterName = getCharacterInfo["CharacterName"].GetString();
                auto characterIt = controller->getCharacter().find(characterName);
                cout << "CharacterIt = " << characterIt->second->getName() << endl;
                if (characterIt != controller->getCharacter().end())
                {
                    writer.String("Name");
                    writer.String(controller->getCharacter().find(characterName)->second->getName().c_str());
                    writer.String("Description");
                    writer.String(controller->getCharacter().find(characterName)->second->getDescr().c_str());
                    writer.String("Health");
                    writer.Int(controller->getCharacter().find(characterName)->second->getHealth());
                    writer.String("AP");
                    writer.Int(controller->getCharacter().find(characterName)->second->getAttackPower());
                    writer.String("DP");
                    writer.Int(controller->getCharacter().find(characterName)->second->getArmorPower());
                    writer.String("Place");
                    writer.String(controller->getCharacter().find(characterName)->second->getPlaceType().c_str());
                    writer.String("Localisation");
                    writer.String(controller->getCharacter().find(characterName)->second->getPlace().c_str());
                }
                else
                {
                    writer.String("Error");
                    writer.String(("Character " + characterName + " hasn't been found.").c_str());
                }
            }
            writer.EndObject();
            writer.EndObject();
        }

        //GetSpaceshipInfo Function
        if (methodName == "GetSpaceshipInfo")
        {
            const rapidjson::Value &getSpaceshipInfo = document["GetSpaceshipInfo"];

            writer.StartObject();
            writer.Key("GetSpaceshipInfo");
            writer.StartObject();
            if (getSpaceshipInfo.HasMember("SpaceshipName"))
            {
                string spaceshipName = getSpaceshipInfo["SpaceshipName"].GetString();
                auto spaceshipIt = controller->getSpaceship().find(spaceshipName);
                if (spaceshipIt != controller->getSpaceship().end())
                {
                    writer.String("SpaceshipName");
                    writer.String(controller->getSpaceship().find(spaceshipName)->second->getName().c_str());
                    writer.String("Crew");
                    writer.StartArray();
                    for (const auto &crewMember : controller->getSpaceship().find(spaceshipName)->second->getCrew())
                    {
                        const auto &character = crewMember.lock();
                        if (character)
                        {
                            writer.StartObject();
                            writer.String("Name");
                            writer.String(character->getName().c_str());
                            writer.String("Health");
                            writer.Int(character->getHealth());
                            writer.String("AP");
                            writer.Int(character->getAttackPower());
                            writer.String("DP");
                            writer.Int(character->getArmorPower());
                            writer.EndObject();
                        }
                    }
                    writer.EndArray();
                }
                else
                {
                    writer.String("Error");
                    writer.String(("Spaceship " + spaceshipName + " hasn't been found.").c_str());
                }
            }
            writer.EndObject();
            writer.EndObject();
        }

        //GetPlanet Function
        if(methodName == "GetPlanetInfo") {
            const rapidjson::Value &getPlanetInfo = document["GetPlanetInfo"];
            writer.StartObject();
            writer.Key("GetPlanetInfo");
            writer.StartObject();
            if(getPlanetInfo.HasMember("PlanetName")){
                string planetName = getPlanetInfo["PlanetName"].GetString();
                auto planetIt = controller->getPlanet().find(planetName);
                if (planetIt != controller->getPlanet().end()){
                    writer.String("PlanetName");
                    writer.String(controller->getPlanet().find(planetName)->second->getName().c_str());

                    writer.String("Residents");
                    writer.StartArray();
                    for (const auto &resident : controller->getPlanet().find(planetName)->second->getResident())
                    {
                        const auto &character = resident.lock();
                        if (character)
                        {
                            writer.StartObject();
                            writer.String("Name");
                            writer.String(character->getName().c_str());
                            writer.String("Health");
                            writer.Int(character->getHealth());
                            writer.String("AP");
                            writer.Int(character->getAttackPower());
                            writer.String("DP");
                            writer.Int(character->getArmorPower());
                            writer.EndObject();
                        }
                    }
                    writer.EndArray();

                }
                else {
                    writer.String("Error");
                    writer.String(("Planet " + planetName + " hasn't been found.").c_str());
                }
            }
            writer.EndObject();
            writer.EndObject();
        }

        //GetQuest Function
        if(methodName == "GetQuestInfo"){

            const rapidjson::Value &getQuestInfo = document["GetQuestInfo"];
            writer.StartObject();
            writer.Key("GetQuestInfo");
            writer.StartObject();

            if (getQuestInfo.HasMember("QuestName")) {
                string questName = getQuestInfo["QuestName"].GetString();
                auto questIt = controller->getQuest().find(questName);

                if(questIt != controller->getQuest().end()){
                    writer.String("QuestName");
                    writer.String(controller->getQuest().find(questName)->second->getName().c_str());
                    writer.String("Description");
                    writer.String(controller->getQuest().find(questName)->second->getDescription().c_str());
                } else {
                    writer.String("Error");
                    writer.String(("Quest " + questName + " hasn't been found.").c_str());
                }
            }
            writer.EndObject();
            writer.EndObject();
        }

        // You can add more conditions for other methods here...
        // writer.EndObject();
        // writer.EndObject();

        // clear buffer data
        memset(buffer, 0, BUFFER_SIZE);

        // response data
        string jsonResponse = sb.GetString();

        // Send the JSON response back to the client
        std::string responseHeader;
        responseHeader += "HTTP/1.1 200 OK\r\n";
        responseHeader += "Content-Type: application/json\r\n";
        responseHeader += "Connection: close\r\n";
        responseHeader += "Content-Length: " + std::to_string(jsonResponse.length()) + "\r\n";
        responseHeader += "\r\n";

        // std::string fullResponse = responseHeader + jsonResponse;

        std::string fullResponse = jsonResponse;

        if (send(conn_id, fullResponse.c_str(), fullResponse.length(), 0) > 0)
        {
            std::cout << "[SEND] " << fullResponse << "\n";
        }
        else
        {
            std::cout << "[WARNING][SEND] " << strerror(errno) << "\n";
        }
    }

    // terminate connection
    close(conn_id);
    std::cout << "[INFO] CONNECTION CLOSED\n";

    // decrease connection counts
    connection--;

    // thread automatically terminate after exit connection handler
    std::cout << "[INFO] THREAD TERMINATED" << std::endl;

    delete reinterpret_cast<ConnectionData *>(data);

    // Recording the end clock tick.
    end = clock();

    // Calculating total time taken by the program.
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

    // print process time
    std::cout << "[TIME] PROCESS COMPLETE IN " << std::fixed << std::setprecision(5) << time_taken;
    std::cout << " SEC\n";

    // print line
    std::cout << "------------------------\n";

    // exiting
    pthread_exit(NULL);
}
