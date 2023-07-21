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

        /*------------------------------------
                    GETs section
        ------------------------------------*/

        // GetCharacterInfo Function
        if (methodName == "getCharacterInfo")
        {
            const rapidjson::Value &getCharacterInfo = document["getCharacterInfo"];

            writer.StartObject();
            writer.Key("getCharacterInfo");
            writer.StartObject();
            if (getCharacterInfo.HasMember("name"))
            {
                string characterName = getCharacterInfo["name"].GetString();
                auto characterIt = controller->getCharacter().find(characterName);
                if (characterIt != controller->getCharacter().end())
                {
                    writer.String("name");
                    writer.String(controller->getCharacter().find(characterName)->second->getName().c_str());
                    writer.String("description");
                    writer.String(controller->getCharacter().find(characterName)->second->getDescr().c_str());
                    writer.String("health");
                    writer.Int(controller->getCharacter().find(characterName)->second->getHealth());
                    writer.String("AP");
                    writer.Int(controller->getCharacter().find(characterName)->second->getAttackPower());
                    writer.String("DP");
                    writer.Int(controller->getCharacter().find(characterName)->second->getArmorPower());
                    writer.String("placeType");
                    writer.String(controller->getCharacter().find(characterName)->second->getPlaceType().c_str());
                    writer.String("place");
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
        if (methodName == "getSpaceshipInfo")
        {
            const rapidjson::Value &getSpaceshipInfo = document["getSpaceshipInfo"];

            writer.StartObject();
            writer.Key("getSpaceshipInfo");
            writer.StartObject();
            if (getSpaceshipInfo.HasMember("name"))
            {
                string spaceshipName = getSpaceshipInfo["name"].GetString();
                auto spaceshipIt = controller->getSpaceship().find(spaceshipName);
                if (spaceshipIt != controller->getSpaceship().end())
                {
                    writer.String("name");
                    writer.String(controller->getSpaceship().find(spaceshipName)->second->getName().c_str());
                    writer.String("crew");
                    writer.StartArray();
                    for (const auto &crewMember : controller->getSpaceship().find(spaceshipName)->second->getCrew())
                    {
                        const auto &character = crewMember.lock();
                        if (character)
                        {
                            writer.StartObject();
                            writer.String("name");
                            writer.String(character->getName().c_str());
                            writer.String("health");
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
        if(methodName == "getPlanetInfo") {
            const rapidjson::Value &getPlanetInfo = document["getPlanetInfo"];
            writer.StartObject();
            writer.Key("getPlanetInfo");
            writer.StartObject();
            if(getPlanetInfo.HasMember("name")){
                string planetName = getPlanetInfo["name"].GetString();
                auto planetIt = controller->getPlanet().find(planetName);
                if (planetIt != controller->getPlanet().end()){
                    writer.String("name");
                    writer.String(controller->getPlanet().find(planetName)->second->getName().c_str());

                    writer.String("residents");
                    writer.StartArray();
                    for (const auto &resident : controller->getPlanet().find(planetName)->second->getResident())
                    {
                        const auto &character = resident.lock();
                        if (character)
                        {
                            writer.StartObject();
                            writer.String("name");
                            writer.String(character->getName().c_str());
                            writer.String("health");
                            writer.Int(character->getHealth());
                            writer.String("AP");
                            writer.Int(character->getAttackPower());
                            writer.String("DP");
                            writer.Int(character->getArmorPower());
                            writer.String("placeType");
                            writer.String(character->getPlaceType().c_str());
                            writer.String("place");
                            writer.String(character->getPlace().c_str());
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
        if(methodName == "getQuestInfo"){

            const rapidjson::Value &getQuestInfo = document["getQuestInfo"];
            writer.StartObject();
            writer.Key("getQuestInfo");
            writer.StartObject();

            if (getQuestInfo.HasMember("name")) {
                string questName = getQuestInfo["name"].GetString();
                auto questIt = controller->getQuest().find(questName);

                if(questIt != controller->getQuest().end()){
                    writer.String("name");
                    writer.String(controller->getQuest().find(questName)->second->getName().c_str());
                    writer.String("description");
                    writer.String(controller->getQuest().find(questName)->second->getDescription().c_str());
                } else {
                    writer.String("Error");
                    writer.String(("Quest " + questName + " hasn't been found.").c_str());
                }
            }
            writer.EndObject();
            writer.EndObject();
        }

        //GetItem Function
        if (methodName == "getItemInfo") {
            const rapidjson::Value& getItemInfo = document["getItemInfo"];
            writer.StartObject();
            writer.Key("getItemInfo");
            writer.StartObject();

            if (getItemInfo.HasMember("name")) {
                std::string itemName = getItemInfo["name"].GetString();
                auto itemIt = controller->getInventory().find(itemName);

                if (itemIt != controller->getInventory().end()) {
                    const auto& item = itemIt->second;
                    writer.String("name");
                    writer.String(item->getName().c_str());
                    writer.String("Description");
                    writer.String(item->getDescription().c_str());
                    writer.String("Effect");
                    writer.Int(item->getEffect());
                } else {
                    writer.String("Error");
                    writer.String(("Item " + itemName + " hasn't been found.").c_str());
                }
            }

            writer.EndObject();
            writer.EndObject();
        }

        //GetCharacters Function
        if (methodName == "getCharacters") {
            writer.StartObject();
            writer.Key("getCharacters");
            writer.StartArray();

            for (const auto& characterPair : controller->getCharacter()) {
                const auto& character = characterPair.second;

                writer.StartObject();
                writer.String("name");
                writer.String(character->getName().c_str());
                writer.String("health");
                writer.Int(character->getHealth());
                writer.String("AP");
                writer.Int(character->getAttackPower());
                writer.String("DP");
                writer.Int(character->getArmorPower());
                writer.String("placetype");
                writer.String(character->getPlaceType().c_str());
                writer.String("place");
                writer.String(character->getPlace().c_str());
                writer.EndObject();
            }
            writer.EndArray();
            writer.EndObject();
        }

        //GetSpaceships Function 
        if (methodName == "getSpaceships") {
            writer.StartObject();
            writer.Key("getSpaceships");
            writer.StartArray();

            for (auto &it : controller->getSpaceship()) {
                writer.StartObject();
                writer.Key("name");
                writer.String(it.second->getName().c_str());

                writer.Key("crew");
                writer.StartArray();

                for (auto crewMemberWeak : it.second->getCrew()) {
                    auto crewMember = crewMemberWeak.lock();
                    if (crewMember) {
                        writer.StartObject();
                        writer.String("name");
                        writer.String(crewMember->getName().c_str());
                        writer.EndObject();
                    }
                }

                writer.EndArray();
                writer.EndObject();
            }

            writer.EndArray();
            writer.EndObject();
        }
        
        //GetQuests Function 
        if (methodName == "getQuests") {
            writer.StartObject();
            writer.Key("getQuests");
            writer.StartArray();
                for (auto &it: controller->getQuest()) {
                    writer.StartObject();
                    writer.String("name");
                    writer.String(it.second->getName().c_str());
                    writer.String("description");
                    writer.String(it.second->getDescription().c_str());
                    writer.EndObject();
                }
            writer.EndArray();
            writer.EndObject();
        }

        //GetInventory Function 
        if (methodName == "getInventory") {
            writer.StartObject();
            writer.Key("getInventory");
            writer.StartArray();
                for (auto &it : controller->getInventory()) {
                    writer.StartObject();
                    writer.String("Name");
                    writer.String(it.second->getName().c_str());
                    writer.String("Description");
                    writer.String(it.second->getDescription().c_str());
                    writer.String("Effect");
                    writer.Int(it.second->getEffect());
                    writer.EndObject();
                }
            writer.EndArray();
            writer.EndObject();
        }
        
        //GetPlanets Function
        if(methodName == "getPlanets"){
            writer.StartObject();
            writer.Key("getPlanets");
            writer.StartArray();

            for (auto &it: controller->getPlanet()) {
                writer.StartObject();
                writer.Key("name");
                writer.String(it.second->getName().c_str());
                writer.Key("residents");
                writer.StartArray();

                for (auto residentList : it.second->getResident()){
                    const auto &resident = residentList.lock();
                    if(resident) {
                        writer.StartObject();
                        writer.String("name");
                        writer.String(resident->getName().c_str());
                        writer.EndObject();
                    }
                }
                writer.EndArray();
                writer.EndObject();
            }
            writer.EndArray();
            writer.EndObject();
        }

        /*------------------------------------
                        ATTACK
        ------------------------------------*/

        //Attack function    
        if (methodName == "attack") {
            const rapidjson::Value &attack = document["attack"];
            if (attack.HasMember("assailant") && attack.HasMember("defender")) {
                std::string assailant = attack["assailant"].GetString();
                std::string defender = attack["defender"].GetString();

                bool result = controller->neutralAttack(assailant, defender);

                writer.StartObject();
                writer.Key("attack");
                writer.StartObject();
                writer.String("assailant");
                writer.String(assailant.c_str());
                writer.String("defender");
                writer.String(defender.c_str());
                writer.String("result");
                writer.Bool(result);
                writer.EndObject();
                writer.EndObject();
            } else {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Les clés 'Assaillant' et 'Défenseur' sont manquantes dans la clé 'Attack'.");
                writer.EndObject();
            }
        }

        /*------------------------------------
                    ADD section
        ------------------------------------*/

        //Add character function 
        if (methodName == "addCharacter") {
            const rapidjson::Value &addCharacter = document["addCharacter"];
            if (addCharacter.HasMember("name") && addCharacter.HasMember("description") &&
                addCharacter.HasMember("health") && addCharacter.HasMember("AP") &&
                addCharacter.HasMember("DP") && addCharacter.HasMember("placeType") &&
                addCharacter.HasMember("place")) {
                
                std::string name = addCharacter["name"].GetString();
                std::string description = addCharacter["description"].GetString();
                int health = addCharacter["health"].GetInt();
                int attackPower = addCharacter["AP"].GetInt();
                int armorPower = addCharacter["DP"].GetInt();
                std::string placeType = addCharacter["placeType"].GetString();
                std::string place = addCharacter["place"].GetString();

                // Créer et ajouter le personnage à la map characterMap
                auto newCharacter = std::make_shared<Character>(name, description, health, attackPower, armorPower, placeType, place);
                controller->addCharacter(newCharacter);

                writer.StartObject();
                writer.Key("addCharacter");
                writer.StartObject();
                writer.String("status");
                writer.String("success");
                writer.EndObject();
                writer.EndObject();
            } else {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'addCharacter'.");
                writer.EndObject();
            }
        }

        //Add planet function 
        if (methodName == "addPlanet") {
            const rapidjson::Value& addPlanet = document["addPlanet"];
            if (addPlanet.HasMember("name") && addPlanet.HasMember("description")) 
            {
                std::string name = addPlanet["name"].GetString();
                std::string description = addPlanet["description"].GetString();
                auto newPlanet = std::make_shared<Planet>(name, description);
                controller->addPlanet(newPlanet);
                writer.StartObject();
                writer.Key("addPlanet");
                writer.StartObject();
                writer.String("status");
                writer.String("success");
                writer.EndObject();
                writer.EndObject();
            } else {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'addPlanet'.");
                writer.EndObject();
            }
        }

        /*------------------------------------
                    DELETE section
        ------------------------------------*/

        //Delete planet function 
        if (methodName == "deletePlanet") {
            const rapidjson::Value& deletePlanet = document["deletePlanet"];
            writer.StartObject();
            writer.Key("deletePlanet");
            if (deletePlanet.HasMember("name")) {
                std::string planetName = deletePlanet["name"].GetString();
                bool result = controller->deletePlanet(planetName);
                if (result) {
                    writer.StartObject();
                    writer.String("success");
                    writer.String(("La planète " + planetName + " a été supprimée.").c_str());
                    writer.EndObject();
                } else {
                    writer.StartObject();
                    writer.String("error");
                    writer.String(("La planète " + planetName + " n'a pas été trouvée.").c_str());
                    writer.EndObject();
                }
            }
            writer.EndObject();
        }
        
        //Delete Character function 
        if (methodName == "deleteCharacter") {
            const rapidjson::Value& deleteCharacter = document["deleteCharacter"];
            writer.StartObject();
            writer.Key("deleteCharacter");
            if (deleteCharacter.HasMember("name")) {
                std::string characterName = deleteCharacter["name"].GetString();
                bool result = controller->deleteCharacter(characterName);
                if (result) {
                    writer.StartObject();
                    writer.String("success");
                    writer.String(("Le personnage " + characterName + " a été supprimé.").c_str());
                    writer.EndObject();
                } else {
                    writer.StartObject();
                    writer.String("error");
                    writer.String(("Le personnage " + characterName + " n'a pas été trouvé.").c_str());
                    writer.EndObject();
                }
            }
            writer.EndObject();
        }

        //DeleteSpaceship function 
        if (methodName == "deleteSpaceship") {
            const rapidjson::Value& deleteSpaceship = document["deleteSpaceship"];
            writer.StartObject();
            writer.Key("deleteSpaceship");
            if (deleteSpaceship.HasMember("name")) {
                std::string spaceshipName = deleteSpaceship["name"].GetString();
                bool result = controller->deleteSpaceship(spaceshipName);
                if (result) {
                    writer.StartObject();
                    writer.String("success");
                    writer.String(("Le vaisseau " + spaceshipName + " a été supprimé.").c_str());
                    writer.EndObject();
                } else {
                    writer.StartObject();
                    writer.String("error");
                    writer.String(("Le vaisseau " + spaceshipName + " n'a pas été trouvé.").c_str());
                    writer.EndObject();
                }
            }
            writer.EndObject();
        }

        //DeleteQuest function 
        if (methodName == "deleteQuest") {
            const rapidjson::Value& deleteQuest = document["deleteQuest"];
            writer.StartObject();
            writer.Key("deleteQuest");
            if (deleteQuest.HasMember("name")) {
                std::string questName = deleteQuest["name"].GetString();
                bool result = controller->deleteQuest(questName);
                if (result) {
                    writer.StartObject();
                    writer.String("success");
                    writer.String(("La mission " + questName + " a été supprimée.").c_str());
                    writer.EndObject();
                } else {
                    writer.StartObject();
                    writer.String("error");
                    writer.String(("La mission " + questName + " n'a pas été trouvée.").c_str());
                    writer.EndObject();
                }
            }
            writer.EndObject();
        }

        
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
