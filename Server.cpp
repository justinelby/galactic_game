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
                    writer.Key("items");
                    writer.StartArray();
                    for (const auto &item : characterIt->second->getInventory())
                    {
                        writer.StartObject();
                        writer.String("name");
                        writer.String(item->getName().c_str());
                        writer.String("description");
                        writer.String(item->getDescription().c_str());
                        writer.String("effect");
                        writer.Int(item->getEffect());
                        writer.EndObject();
                    }
                    writer.EndArray();
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

        // GetCharacterInfo Function
        if (methodName == "getEnemyInfo")
        {
            const rapidjson::Value &getEnemyInfo = document["getEnemyInfo"];

            writer.StartObject();
            writer.Key("getEnemyInfo");
            writer.StartObject();
            if (getEnemyInfo.HasMember("name"))
            {
                string enemyName = getEnemyInfo["name"].GetString();
                auto enemyIt = controller->getEnemy().find(enemyName);
                if (enemyIt != controller->getEnemy().end())
                {
                    writer.String("name");
                    writer.String(controller->getEnemy().find(enemyName)->second->getName().c_str());
                    writer.String("description");
                    writer.String(controller->getEnemy().find(enemyName)->second->getDescr().c_str());
                    writer.String("health");
                    writer.Int(controller->getEnemy().find(enemyName)->second->getHealth());
                    writer.String("AP");
                    writer.Int(controller->getEnemy().find(enemyName)->second->getAttackPower());
                    writer.String("DP");
                    writer.Int(controller->getEnemy().find(enemyName)->second->getArmorPower());
                    writer.String("placeType");
                    writer.String(controller->getEnemy().find(enemyName)->second->getPlaceType().c_str());
                    writer.String("place");
                    writer.String(controller->getEnemy().find(enemyName)->second->getPlace().c_str());
                }
                else
                {
                    writer.String("Error");
                    writer.String(("Enemy " + enemyName + " hasn't been found.").c_str());
                }
            }
            writer.EndObject();
            writer.EndObject();
        }

        // GetSpaceshipInfo Function
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

        // GetPlanet Function
        if (methodName == "getPlanetInfo")
        {
            const rapidjson::Value &getPlanetInfo = document["getPlanetInfo"];
            writer.StartObject();
            writer.Key("getPlanetInfo");
            writer.StartObject();
            if (getPlanetInfo.HasMember("name"))
            {
                string planetName = getPlanetInfo["name"].GetString();
                auto planetIt = controller->getPlanet().find(planetName);
                if (planetIt != controller->getPlanet().end())
                {
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
                else
                {
                    writer.String("Error");
                    writer.String(("Planet " + planetName + " hasn't been found.").c_str());
                }
            }
            writer.EndObject();
            writer.EndObject();
        }

        // GetQuest Function
        if (methodName == "getQuestInfo")
        {

            const rapidjson::Value &getQuestInfo = document["getQuestInfo"];
            writer.StartObject();
            writer.Key("getQuestInfo");
            writer.StartObject();

            if (getQuestInfo.HasMember("name"))
            {
                string questName = getQuestInfo["name"].GetString();
                auto questIt = controller->getQuest().find(questName);

                if (questIt != controller->getQuest().end())
                {
                    writer.String("name");
                    writer.String(controller->getQuest().find(questName)->second->getName().c_str());
                    writer.String("description");
                    writer.String(controller->getQuest().find(questName)->second->getDescription().c_str());
                    bool isCompleted = controller->getQuest().find(questName)->second->getIsCompleted();
                    writer.String("isCompleted");
                    writer.Bool(isCompleted);
                }
                else
                {
                    writer.String("Error");
                    writer.String(("Quest " + questName + " hasn't been found.").c_str());
                }
            }
            writer.EndObject();
            writer.EndObject();
        }

        // GetItem Function
        if (methodName == "getItemInfo")
        {
            const rapidjson::Value &getItemInfo = document["getItemInfo"];
            writer.StartObject();
            writer.Key("getItemInfo");
            writer.StartObject();

            if (getItemInfo.HasMember("name"))
            {
                std::string itemName = getItemInfo["name"].GetString();
                auto itemIt = controller->getInventory().find(itemName);

                if (itemIt != controller->getInventory().end())
                {
                    const auto &item = itemIt->second;
                    writer.String("name");
                    writer.String(item->getName().c_str());
                    writer.String("Description");
                    writer.String(item->getDescription().c_str());
                    writer.String("Effect");
                    writer.Int(item->getEffect());
                }
                else
                {
                    writer.String("Error");
                    writer.String(("Item " + itemName + " hasn't been found.").c_str());
                }
            }

            writer.EndObject();
            writer.EndObject();
        }

        // GetCharacters Function
        if (methodName == "getCharacters")
        {
            writer.StartObject();
            writer.Key("getCharacters");
            writer.StartArray();

            for (const auto &characterPair : controller->getCharacter())
            {
                const auto &character = characterPair.second;

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

        // GetEnemies Function
        if (methodName == "getEnemies")
        {
            writer.StartObject();
            writer.Key("getEnemies");
            writer.StartArray();

            for (const auto &enemyPair : controller->getEnemy())
            {
                const auto &enemy = enemyPair.second;

                writer.StartObject();
                writer.String("name");
                writer.String(enemy->getName().c_str());
                writer.String("health");
                writer.Int(enemy->getHealth());
                writer.String("AP");
                writer.Int(enemy->getAttackPower());
                writer.String("DP");
                writer.Int(enemy->getArmorPower());
                writer.String("placetype");
                writer.String(enemy->getPlaceType().c_str());
                writer.String("place");
                writer.String(enemy->getPlace().c_str());
                writer.EndObject();
            }
            writer.EndArray();
            writer.EndObject();
        }

        // GetSpaceships Function
        if (methodName == "getSpaceships")
        {
            writer.StartObject();
            writer.Key("getSpaceships");
            writer.StartArray();

            for (auto &it : controller->getSpaceship())
            {
                writer.StartObject();
                writer.Key("name");
                writer.String(it.second->getName().c_str());

                writer.Key("crew");
                writer.StartArray();

                for (auto crewMemberWeak : it.second->getCrew())
                {
                    auto crewMember = crewMemberWeak.lock();
                    if (crewMember)
                    {
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

        // GetQuests Function
        if (methodName == "getQuests")
        {
            writer.StartObject();
            writer.Key("getQuests");
            writer.StartArray();
            for (auto it : controller->getQuest())
            {
                writer.StartObject();
                writer.String("name");
                writer.String(it.second->getName().c_str());
                writer.String("description");
                writer.String(it.second->getDescription().c_str());
                /* writer.String("isCompleted");
                writer.Bool(it.second->getIsCompleted()); */
                writer.EndObject();
            }
            writer.EndArray();
            writer.EndObject();
        }

        // GetInventory Function
        if (methodName == "getInventory")
        {
            writer.StartObject();
            writer.Key("getInventory");
            writer.StartArray();
            for (auto &it : controller->getInventory())
            {
                if (it.second != nullptr) {
                writer.StartObject();
                writer.String("name");
                writer.String(it.second->getName().c_str());
                writer.String("description");
                writer.String(it.second->getDescription().c_str());
                writer.String("effect");
                writer.Int(it.second->getEffect());
                writer.EndObject();
                }
            }
            writer.EndArray();
            writer.EndObject();
        }

        // GetPlanets Function
        if (methodName == "getPlanets")
        {
            writer.StartObject();
            writer.Key("getPlanets");
            writer.StartArray();

            for (auto &it : controller->getPlanet())
            {
                writer.StartObject();
                writer.Key("name");
                writer.String(it.second->getName().c_str());
                writer.Key("residents");
                writer.StartArray();

                for (auto residentList : it.second->getResident())
                {
                    const auto &resident = residentList.lock();
                    if (resident)
                    {
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

        // Attack function
        if (methodName == "attack")
        {
            const rapidjson::Value &attack = document["attack"];
            if (attack.HasMember("assailant") && attack.HasMember("defender"))
            {
                std::string assailant = attack["assailant"].GetString();
                std::string defender = attack["defender"].GetString();

                bool result = controller->neutralAttack(assailant, defender);
                int remainingHealth = controller->getCharacter().find(defender)->second->getHealth();
              
                if(result){
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
                writer.Key("attack");
                writer.StartObject();
                writer.String("assailant");
                writer.String(assailant.c_str());
                writer.String("defender");
                writer.String(defender.c_str());
                writer.String("result");
                writer.Bool(result);
                writer.String((to_string(remainingHealth)).c_str());
                writer.EndObject();
                writer.EndObject();
                }
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Les clés 'Assaillant' et 'Défenseur' sont manquantes dans la clé 'Attack'.");
                writer.EndObject();
            }
        }

        /*------------------------------------
                    ITEM MANAGER section
         -----------------------------------*/

        // Swapping items function
        if (methodName == "swapItems")
        {
            const rapidjson::Value &swapItems = document["swapItems"];
            if (swapItems.HasMember("characterName") && swapItems.HasMember("itemOnGround") && swapItems.HasMember("itemToReplace"))
            {
                std::string character = swapItems["characterName"].GetString();
                std::string itemOnGround = swapItems["itemOnGround"].GetString();
                std::string itemToReplace = swapItems["itemToReplace"].GetString();

                bool result = controller->swapItems(character, itemOnGround, itemToReplace);

                writer.StartObject();
                writer.Key("swapItems");

                if (result)
                {
                    writer.StartObject();
                    writer.String("Success");
                    writer.String((itemToReplace + " a bien été échangé avec " + itemOnGround + ". ").c_str());
                    writer.EndObject();
                }
                else
                {
                    writer.StartObject();
                    writer.String("Failure");
                    writer.String(("Inventory not full, you can still grab an item without having to swap one of yours. Please call addToCharacterInventory(" + character + ", " + itemOnGround + ")").c_str());
                    writer.EndObject();
                }
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Une ou plusieurs clés 'characterName', 'itemOnGround' et 'itemToReplace' sont manquantes dans la clé 'swapItems'.");
                writer.EndObject();
            }
            writer.EndObject();
        }

        /*------------------------------------
                    ADD section
        ------------------------------------*/

        // Add character function
        if (methodName == "addCharacter")
        {
            const rapidjson::Value &addCharacter = document["addCharacter"];
            if (addCharacter.HasMember("name") && addCharacter.HasMember("description") &&
                addCharacter.HasMember("health") && addCharacter.HasMember("AP") &&
                addCharacter.HasMember("DP") && addCharacter.HasMember("placeType") &&
                addCharacter.HasMember("place"))
            {

                std::string name = addCharacter["name"].GetString();
                std::string description = addCharacter["description"].GetString();
                int health = addCharacter["health"].GetInt();
                int attackPower = addCharacter["AP"].GetInt();
                int armorPower = addCharacter["DP"].GetInt();
                std::string placeType = addCharacter["placeType"].GetString();
                std::string place = addCharacter["place"].GetString();

                // Créer et ajouter le personnage à la map characterMap
                auto newCharacter = std::make_shared<Character>(name, description, health, attackPower, armorPower, placeType, place);

                // Vérifier si un personnage avec le même nom existe déjà
                auto it = controller->getCharacter().find(name);
                if (it != controller->getCharacter().end())
                {
                    writer.StartObject();
                    writer.Key("addCharacter");
                    writer.StartObject();
                    writer.String("status");
                    writer.String("failure");
                    writer.String("message");
                    writer.String("Personnage déjà existant. Veuillez reessayer avec un autre nom.");
                    writer.EndObject();
                    writer.EndObject();
                }
                else
                {
                    controller->getCharacter()[name] = newCharacter;
                    writer.StartObject();
                    writer.Key("addCharacter");
                    writer.StartObject();
                    writer.String("status");
                    writer.String("success");
                    writer.String("message");
                    writer.String("Personnage ajouté avec succès.");
                    writer.EndObject();
                    writer.EndObject();
                }
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'addCharacter'.");
                writer.EndObject();
            }
        }

        // Add enemy function
        if (methodName == "addEnemy")
        {
            const rapidjson::Value &addEnemy = document["addEnemy"];
            if (addEnemy.HasMember("name") && addEnemy.HasMember("description") &&
                addEnemy.HasMember("health") && addEnemy.HasMember("AP") &&
                addEnemy.HasMember("DP") && addEnemy.HasMember("placeType") &&
                addEnemy.HasMember("place"))
            {

                std::string name = addEnemy["name"].GetString();
                std::string description = addEnemy["description"].GetString();
                int health = addEnemy["health"].GetInt();
                int attackPower = addEnemy["AP"].GetInt();
                int armorPower = addEnemy["DP"].GetInt();
                std::string placeType = addEnemy["placeType"].GetString();
                std::string place = addEnemy["place"].GetString();

                // Créer et ajouter l'ennemi à la map enemyMap
                auto newEnemy = std::make_shared<Enemy>(name, description, health, attackPower, armorPower, placeType, place);

                // Vérifier si un ennemi avec le même nom existe déjà
                auto it = controller->getEnemy().find(name);
                if (it != controller->getEnemy().end())
                {
                    writer.StartObject();
                    writer.Key("addEnemy");
                    writer.StartObject();
                    writer.String("status");
                    writer.String("failure");
                    writer.String("message");
                    writer.String("Ennemi déjà existant. Veuillez reessayer avec un autre nom.");
                    writer.EndObject();
                    writer.EndObject();
                }
                else
                {
                    controller->getEnemy()[name] = newEnemy;
                    writer.StartObject();
                    writer.Key("addEnemy");
                    writer.StartObject();
                    writer.String("status");
                    writer.String("success");
                    writer.String("message");
                    writer.String("Ennemi ajouté avec succès.");
                    writer.EndObject();
                    writer.EndObject();
                }
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'addEnemy'.");
                writer.EndObject();
            }
        }

        // Add quest function
        if (methodName == "addQuest")
        {
            const rapidjson::Value &addQuest = document["addQuest"];
            if (addQuest.HasMember("name") && addQuest.HasMember("description"))
            {
                std::string name = addQuest["name"].GetString();
                std::string description = addQuest["description"].GetString();
                bool isCompleted = addQuest["isCompleted"].GetBool(); // Charge la propriété "isCompleted" comme booléen

                auto newQuest = std::make_shared<Quest>(name, description, isCompleted);
                controller->addQuest(newQuest);

                writer.StartObject();
                writer.Key("addQuest");
                writer.StartObject();
                writer.String("status");
                writer.String("success");
                writer.EndObject();
                writer.EndObject();
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'addQuest'.");
                writer.EndObject();
            }
        }

        // Add planet function
        if (methodName == "addPlanet")
        {
            const rapidjson::Value &addPlanet = document["addPlanet"];
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
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'addPlanet'.");
                writer.EndObject();
            }
        }

        // Add spaceship function
        if (methodName == "addSpaceship")
        {
            const rapidjson::Value &addSpaceship = document["addSpaceship"];
            if (addSpaceship.HasMember("name"))
            {
                std::string name = addSpaceship["name"].GetString();
                auto newSpaceship = std::make_shared<Spaceship>(name);
                controller->addSpaceship(newSpaceship);
                writer.StartObject();
                writer.Key("addSpaceship");
                writer.StartObject();
                writer.String("status");
                writer.String("success");
                writer.EndObject();
                writer.EndObject();
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'addSpaceship'.");
                writer.EndObject();
            }
        }

        if (methodName == "addToCharacterInventory")
        {
            bool itemAdded = false;
            const rapidjson::Value &addToInventory = document["addToCharacterInventory"];
            if (addToInventory.HasMember("charName") && addToInventory.HasMember("itemName"))
            {
                std::string charName = addToInventory["charName"].GetString();
                std::string itemName = addToInventory["itemName"].GetString();
        
                // Appeler la fonction pour ajouter l'objet à l'inventaire du personnage
                bool check1 = controller->isCharacterExists(charName),
                check2 = controller->isItemExists(itemName);
                if(check1 && check2) {
                    controller->addToCharacterInventory(charName, itemName);
        
                    writer.StartObject();
                    writer.Key("addToCharacterInventory");
                    writer.StartObject();
        
                    auto characterIt = controller->getCharacter().find(charName);
        
                    // Vérifier la valeur de itemAdded et ajouter le message approprié dans la réponse JSON
                    if (characterIt->second->getInventory().size() < 5) {
                        writer.String("status");
                        writer.String("success");
                    } else if (characterIt->second->getInventory().size() >= 5) {
                        writer.String("status");
                        writer.String("failed : inventory is full, please use swapItem method");
                    }
        
                    writer.EndObject();
                    writer.EndObject();
                }
                else {
                    writer.StartObject();
                    writer.Key("Error");
                    writer.String("Invalid entries, please write an existing character and/or item.");
                    writer.EndObject();
        
                }
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'addToCharacterInventory'.");
                writer.EndObject();
            }
        }


        // Add addToGameInventory function
        if (methodName == "addToGameInventory")
        {
            const rapidjson::Value &addToGameInventory = document["addToGameInventory"];
            if (addToGameInventory.HasMember("name") && addToGameInventory.HasMember("description") &&
                addToGameInventory.HasMember("effect"))
            {
                std::string name = addToGameInventory["name"].GetString();
                std::string description = addToGameInventory["description"].GetString();
                int effect = addToGameInventory["effect"].GetInt();

                bool check = controller->isItemExists(name);
                if(!check) {    // if an item with the same name doesn't already exist
                    // Créer et ajouter le personnage à la map characterMap
                    auto newItem = make_unique<Item>(name, description, effect);
                    controller->addToGameInventory(newItem);
                    writer.StartObject();
                    writer.Key("addToGameInventory");
                    writer.StartObject();
                    writer.String("status");
                    writer.String("success");
                    writer.EndObject();
                    writer.EndObject();
                }
                else {
                    writer.StartObject();
                    writer.Key("Error");
                    writer.String("An item already exists with this name. Please change your new item's name");
                    writer.EndObject();
                }
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'newItem'.");
                writer.EndObject();
            }
        }
        /*------------------------------------
                    DELETE section
        ------------------------------------*/

        // Delete planet function
        if (methodName == "deletePlanet")
        {
            const rapidjson::Value &deletePlanet = document["deletePlanet"];
            writer.StartObject();
            writer.Key("deletePlanet");
            if (deletePlanet.HasMember("name"))
            {
                std::string planetName = deletePlanet["name"].GetString();
                bool result = controller->deletePlanet(planetName);
                if (result)
                {
                    writer.StartObject();
                    writer.String("success");
                    writer.String(("La planète " + planetName + " a été supprimée.").c_str());
                    writer.EndObject();
                }
                else
                {
                    writer.StartObject();
                    writer.String("error");
                    writer.String(("La planète " + planetName + " n'a pas été trouvée.").c_str());
                    writer.EndObject();
                }
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'deletePlanet'.");
                writer.EndObject();
            }
            writer.EndObject();
        }

        // Delete Character function
        if (methodName == "deleteCharacter")
        {
            const rapidjson::Value &deleteCharacter = document["deleteCharacter"];
            writer.StartObject();
            writer.Key("deleteCharacter");
            if (deleteCharacter.HasMember("name"))
            {
                std::string characterName = deleteCharacter["name"].GetString();
                bool result = controller->deleteCharacter(characterName);
                if (result)
                {
                    writer.StartObject();
                    writer.String("success");
                    writer.String(("Le personnage " + characterName + " a été supprimé.").c_str());
                    writer.EndObject();
                }
                else
                {
                    writer.StartObject();
                    writer.String("error");
                    writer.String(("Le personnage " + characterName + " n'a pas été trouvé.").c_str());
                    writer.EndObject();
                }
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'deleteCharacter'.");
                writer.EndObject();
            }
            writer.EndObject();
        }

        // DeleteSpaceship function
        if (methodName == "deleteSpaceship")
        {
            const rapidjson::Value &deleteSpaceship = document["deleteSpaceship"];
            writer.StartObject();
            writer.Key("deleteSpaceship");
            if (deleteSpaceship.HasMember("name"))
            {
                std::string spaceshipName = deleteSpaceship["name"].GetString();
                bool result = controller->deleteSpaceship(spaceshipName);
                if (result)
                {
                    writer.StartObject();
                    writer.String("success");
                    writer.String(("Le vaisseau " + spaceshipName + " a été supprimé.").c_str());
                    writer.EndObject();
                }
                else
                {
                    writer.StartObject();
                    writer.String("error");
                    writer.String(("Le vaisseau " + spaceshipName + " n'a pas été trouvé.").c_str());
                    writer.EndObject();
                }
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'deleteSpaceship'.");
                writer.EndObject();
            }
            writer.EndObject();
        }

        // DeleteQuest function
        if (methodName == "deleteQuest")
        {
            const rapidjson::Value &deleteQuest = document["deleteQuest"];
            writer.StartObject();
            writer.Key("deleteQuest");
            if (deleteQuest.HasMember("name"))
            {
                std::string questName = deleteQuest["name"].GetString();
                bool result = controller->deleteQuest(questName);
                if (result)
                {
                    writer.StartObject();
                    writer.String("success");
                    writer.String(("La mission " + questName + " a été supprimée.").c_str());
                    writer.EndObject();
                }
                else
                {
                    writer.StartObject();
                    writer.String("error");
                    writer.String(("La mission " + questName + " n'a pas été trouvée.").c_str());
                    writer.EndObject();
                }
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'deleteQuest'.");
                writer.EndObject();
            }
            writer.EndObject();
        }

        // DeleteItem function
        if (methodName == "deleteItemToCharacterInventory")
        {
            const rapidjson::Value &deleteItemToCharacterInventory = document["deleteItemToCharacterInventory"];
            writer.StartObject();
            writer.Key("deleteItemToCharacterInventory");
            if (deleteItemToCharacterInventory.HasMember("name") || deleteItemToCharacterInventory.HasMember("item"))
            {
                std::string characterName = deleteItemToCharacterInventory["name"].GetString();
                std::string itemName = deleteItemToCharacterInventory["item"].GetString();
                bool result = controller->deleteItemToCharacterInventory(characterName, itemName);
                if (result)
                {
                    writer.StartObject();
                    writer.String("success");
                    writer.String(("L'objet " + itemName + " a été supprimée de l'inventaire de " + characterName).c_str());
                    writer.EndObject();
                }
                else
                {
                    writer.StartObject();
                    writer.String("error");
                    writer.String(("L'objet' " + itemName + " n'a pas été trouvé.").c_str());
                    writer.EndObject();
                }
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquants dans la clé 'deleteItemToCharacterInventory'.");
                writer.EndObject();
            }
            writer.EndObject();
        }

        /*------------------------------------
                    GAMEPLAY section
        ------------------------------------*/
        if (methodName == "saveGame")
        {
            controller->saveGame();
            writer.StartObject();
            writer.Key("Success");
            writer.String("Les données du jeu ont bien été enregistrées.");
            writer.EndObject();
        }

        if (methodName == "resetGame")
        {
            controller->resetGame();
            writer.StartObject();
            writer.Key("Success");
            writer.String("Les données du jeu ont bien été restaurées.");
            writer.EndObject();
        }

        if (methodName == "useItem")
        {
            const rapidjson::Value &useItem = document["useItem"];
            writer.StartObject();
            writer.Key("useItem");

            if (useItem.HasMember("characterName") || useItem.HasMember("itemName"))
            {
                std::string characterName = useItem["characterName"].GetString();
                std::string itemName = useItem["itemName"].GetString();
                controller->useItem(characterName, itemName);
                writer.StartObject();
                writer.Key("Success");
                writer.String(("L'objet " + itemName + " a bien été utilisé par " + characterName).c_str());
                writer.EndObject();
            }
            else
            {
                writer.StartObject();
                writer.Key("Error");
                writer.String("Certains champs sont manquantes dans la clé 'useItem'.");
                writer.EndObject();
            }
            writer.EndObject();
        }

        if(methodName == "setIsCompleted"){
            const rapidjson::Value &setIsCompleted = document["setIsCompleted"];
            writer.StartObject();
            writer.Key("setIsCompleted");

            if(setIsCompleted.HasMember("name") || setIsCompleted.HasMember("isCompleted")){
                std::string name = setIsCompleted["name"].GetString();
                bool isCompleted = setIsCompleted["isCompleted"].GetBool();
                controller->getQuest().find(name)->second->setIsCompleted(name, isCompleted);
                writer.StartObject();
                writer.Key("Success");
                writer.String(("La mission " + name + " a bien été mise à jour.").c_str());
                writer.EndObject();
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