//
// Created by justi on 27/06/2023.
//
#define DEBUG

#include "Controller.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "Character.h"
#include "Spaceship.h"
#include "Planet.h"
#include "./rapidjson-master/include/rapidjson/document.h"

using namespace std;

Controller::Controller(string &loadedFile, string &savedFile) : gameFile(loadedFile), savedFile(savedFile) {
}

map<string, shared_ptr<Quest>> Controller::getQuest() {
    return questMap;
}

map<string, shared_ptr<Character>> Controller::getCharacter() {
    return characterMap;
}

map<string, shared_ptr<Enemy>> Controller::getEnemy() {
    return enemyMap;
}

map<string, shared_ptr<Planet>> Controller::getPlanet() {
    return planetMap;
}

map<string, shared_ptr<Spaceship>> Controller::getSpaceship() {
    return spaceshipMap;
}

string Controller::characterToString() {
    ostringstream oss;
    for (const auto &pair: characterMap) {
        auto c = pair.second;
        oss << "Character;" << c->getName() << ";" << c->getDescr() << ";" << c->getHealth() << ";"
            << c->getAttackPower() << ";" << c->getArmorPower() << ";" << c->getPlaceType() << ";" << c->getPlace()
            << "\n";
    }
    return oss.str();
}

string Controller::planetToString() {
    ostringstream oss;
    for (const auto &pair: planetMap) {
        auto p = pair.second;
        oss << "Planet;" << p->getName() << ";" << p->getDescription() << "\n";
    }
    return oss.str();
}

string Controller::spaceshipToString() {
    ostringstream oss;
    for (const auto &pair: spaceshipMap) {
        auto p = pair.second;
        oss << "Spaceship;" << p->getName() << endl;
    }
    return oss.str();
}

string Controller::questToString() {
    ostringstream oss;
    for (const auto &pair: questMap) {
        auto m = pair.second;
        oss << "Mission;" << m->getName() << ";" << m->getDescription() << "\n";
    }
    return oss.str();
}

void Controller::loadGame() {
    srand(static_cast <unsigned int> (time(NULL)));     // generating new random seed
    ifstream file(gameFile);

    if (!file.is_open()) {
        cout << "Le fichier de chargement du jeu ne s'est pas ouvert" << endl;
    }
// Lecture du contenu du fichier JSON
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Création du document JSON
    rapidjson::Document document;
    document.Parse(jsonContent.c_str());

    // Chargement des données à partir du document JSON

    // Chargement des character
    if (document.HasMember("character")) {
        const rapidjson::Value& characters = document["character"];
        if (characters.IsArray()) {
            for (rapidjson::SizeType i = 0; i < characters.Size(); i++) {
                const rapidjson::Value& character = characters[i];
                // Extraire les valeurs des propriétés du personnage
                std::string name = character["Nom"].GetString();
                std::string description = character["Description"].GetString();
                int health = character["Santé"].GetInt();
                int attackPower = character["Puissance d'attaque"].GetInt();
                int armorPower = character["Puissance d'armure"].GetInt();
                std::string placeType = character["Type de lieu"].GetString();
                std::string place = character["Lieu"].GetString();
                // Créer et ajouter le personnage à la map characterMap
                auto newCharacter = std::make_shared<Character>(name, description, health, attackPower, armorPower, placeType, place);
                addCharacter(newCharacter);
            }
        }
    }

    // Chargement des planet
    if (document.HasMember("planet")) {
        const rapidjson::Value& planets = document["planet"];
        if (planets.IsArray()) {
            for (rapidjson::SizeType i = 0; i < planets.Size(); i++) {
                const rapidjson::Value& planet = planets[i];
                // Extraire les valeurs des propriétés de la planète
                std::string name = planet["Nom"].GetString();
                std::string description = planet["Description"].GetString();
                // Créer et ajouter la planète à la map planetMap
                auto newPlanet = std::make_shared<Planet>(name, description);
                addPlanet(newPlanet);
            }
        }
    }

    // Chargement des spaceship
    if (document.HasMember("spaceship")) {
        const rapidjson::Value& spaceships = document["spaceship"];
        if (spaceships.IsArray()) {
            for (rapidjson::SizeType i = 0; i < spaceships.Size(); i++) {
                const rapidjson::Value& spaceship = spaceships[i];
                // Extraire les valeurs des propriétés du vaisseau spatial
                std::string name = spaceship["Nom"].GetString();
                // Créer et ajouter le vaisseau spatial à la map spaceshipMap
                auto newSpaceship = std::make_shared<Spaceship>(name);
                addSpaceship(newSpaceship);
            }
        }
    }

    // Chargement des quest
    if (document.HasMember("quest")) {
        const rapidjson::Value& quests = document["quest"];
        if (quests.IsArray()) {
            for (rapidjson::SizeType i = 0; i < quests.Size(); i++) {
                const rapidjson::Value& quest = quests[i];
                // Extraire les valeurs des propriétés de la quête
                std::string name = quest["Nom"].GetString();
                std::string description = quest["Description"].GetString();
                // Créer et ajouter la quête à la map questMap
                auto newQuest = std::make_shared<Quest>(name, description);
                addQuest(newQuest);
            }
        }
    }
}

void Controller::loadActions(string actionsFile) {
    ifstream file(actionsFile);

    if (!file.is_open()) {
        cout << "Le fichier du chargement des fonctions ne s'est pas ouvert" << endl;
    }
// Lecture du contenu du fichier JSON
    std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Création du document JSON
    rapidjson::Document document;
    document.Parse(jsonContent.c_str());

    // Chargement des données à partir du document JSON

    // Vérifier si le document JSON contient la clé "attack"
    if (document.HasMember("attack")) {
        const rapidjson::Value& attack = document["attack"];

        // Vérifier si les clés "Assaillant" et "Défenseur" sont présentes
        if (attack.HasMember("Assaillant") && attack.HasMember("Défenseur")) {
            std::string assailant = attack["Assaillant"].GetString();
            std::string defender = attack["Défenseur"].GetString();

            // Appeler la fonction neutralAttack avec les noms des personnages assailant et defender
            bool result = neutralAttack(assailant, defender);
            if (result) {
                std::cout << "Le personnage " << defender << " a été éliminé.\n";
            } else {
                std::cout << "Le personnage " << defender << " a subi des dégâts.\n";
            }
        } else {
            std::cout << "Les clés 'Assaillant' et 'Défenseur' sont manquantes dans la clé 'attack'.\n";
        }
    } else {
        std::cout << "Le document JSON ne contient pas la clé 'attack'.\n";
    }
}

void Controller::saveGame() {
    //Ecriture du fichier de sauvegarde
    ofstream file(savedFile);
    file << planetToString() << spaceshipToString() << characterToString() << questToString() << endl;
}

void Controller::addCharacter(const shared_ptr<Character> &newCharacter) {
    characterMap[newCharacter->getName()] = newCharacter;
    // Ajout du personnage à l'équipage du vaisseau auquel il est associé
    for (auto &ship: spaceshipMap) {
        if (newCharacter->getPlace() == ship.second->getName()) {
            ship.second->addCrewMember(characterMap[newCharacter->getName()]);
            break;
        }
    }

    // Ajout du personnage aux habitants de la planete auquel il est associé
    for (auto &pla: planetMap) {
        if (newCharacter->getPlace() == pla.second->getName()) {
            pla.second->addNewPlanetResident(characterMap[newCharacter->getName()]);
            break;
        }
    }
}

// redundancy
void Controller::addEnemy(const shared_ptr<Enemy> &newEnemy) {
    enemyMap[newEnemy->getName()] = newEnemy;
    // Ajout du personnage à l'équipage du vaisseau auquel il est associé
    for (auto &ship: spaceshipMap) {
        if (newEnemy->getPlace() == ship.second->getName()) {
            ship.second->addEnemyCrewMember(enemyMap[newEnemy->getName()]);
            break;
        }
    }

    // Ajout de l'enemy aux habitants de la planete auquel il est associé
    for (auto &pla: planetMap) {
        if (newEnemy->getPlace() == pla.second->getName()) {
            pla.second->addNewPlanetEnemyResident(enemyMap[newEnemy->getName()]);
            break;
        }
    }
}

void Controller::addSpaceship(const shared_ptr<Spaceship> &newSpaceship) {
    spaceshipMap[newSpaceship->getName()] = newSpaceship;
}

void Controller::addPlanet(const shared_ptr<Planet> &newPlanet) {
    planetMap[newPlanet->getName()] = newPlanet;
}

void Controller::addQuest(const shared_ptr<Quest> &newMission) {
    questMap[newMission->getName()] = newMission;
}

void Controller::cleanWeakPtr(
        vector<weak_ptr<Character>> &vec) { //Nettoyer les weak ptr qui n'ont plus de share ptr vers lesquels pointer
    //on parcourt les weak ptr du vecteur
    for (auto it = vec.begin(); it != vec.end();) {
        if (it->lock() == 0) {
            // Le weakptr a expiré, on le supprime de la liste
            it = vec.erase(it);
        } else {
            // Le weakptr est toujours valide, on passe au suivant
            ++it;
        }
    }
    vec.clear();
}

bool Controller::deleteCharacter(const string& name) {
    // Rechercher le personnage dans la map characterMap
    auto it = characterMap.find(name);
    auto it2 = enemyMap.find(name);

    if (it != characterMap.end()) {
        string characterName = it->first;
        // Vérifier si le pointeur de personnage est nul
        if (characterMap[characterName]) {
            // Obtenir le type de lieu où se situe le personnage
            string typePlace = characterMap[characterName]->getPlaceType();
            // Si le personnage est sur une planète
            if (typePlace == "Planet") {
                // Obtenir la planète associée au personnage
                string place = characterMap[characterName]->getPlace();
                auto planetIt = planetMap.find(place);
                if (planetIt != planetMap.end()) {
                    auto planet = planetIt->second;
                    // Supprimer le personnage de la liste des résidents de la planète
                    cleanWeakPtr(planet->getResident());
                }
            }
                // Si le personnage est sur un vaisseau
            else if (typePlace == "Spaceship") {
                string place = characterMap[characterName]->getPlace();
                auto spaceshipIt = spaceshipMap.find(place);
                if (spaceshipIt != spaceshipMap.end()) {
                    auto spaceship = spaceshipIt->second;
                    cleanWeakPtr(spaceship->getCrew());
                }
            }
        }
        characterMap.erase(characterName);
        return true;
    }
    else if (it2 != enemyMap.end()) {
        string enemyName = it2->first;
        // Vérifier si le pointeur de personnage est nul
        if (enemyMap[enemyName]) {
            // Obtenir le type de lieu où se situe le personnage
            string typePlace = enemyMap[enemyName]->getPlaceType();
            // Si le personnage est sur une planète
            if (typePlace == "Planet") {
                // Obtenir la planète associée au personnage
                string place = enemyMap[enemyName]->getPlace();
                auto planetIt = planetMap.find(place);
                if (planetIt != planetMap.end()) {
                    auto planet = planetIt->second;
                    // Supprimer le personnage de la liste des résidents de la planète
                    cleanWeakPtr(planet->getResident());
                }
            }
                // Si le personnage est sur un vaisseau
            else if (typePlace == "Spaceship") {
                string place = enemyMap[enemyName]->getPlace();
                auto spaceshipIt = spaceshipMap.find(place);
                if (spaceshipIt != spaceshipMap.end()) {
                    auto spaceship = spaceshipIt->second;
                    cleanWeakPtr(spaceship->getCrew());
                }
            }
        }
        enemyMap.erase(enemyName);
        return true;
    }
    else {
        return false;
    }
}

bool Controller::deleteSpaceship(const string& name) {
    // Rechercher le vaisseau dans la map spaceshipMap
    auto it = spaceshipMap.find(name);
    if (it == spaceshipMap.end()) {
        return false;
    }
    else {
        string spaceshipName = it->first;
        if (spaceshipMap[spaceshipName]) {
            // Parcourir les membres de l'équipage du vaisseau
            for (auto member : it->second->getCrew()) {
                if (auto character = dynamic_pointer_cast<Character>(member.lock())) {
                    // Vérifier si c'est un ennemi
                    if (auto enemy = dynamic_pointer_cast<Enemy>(character)) {
                        cout << "Suppression de l'ennemi : " << enemy->getName() << endl;
                        enemyMap.erase(enemy->getName());
                    }
                    else {
                        // Supprimer le personnage de la map characterMap
                        cout << "Suppression du personnage: " << character->getName() << endl;
                        characterMap.erase(character->getName());
                    }
                }
            }
            // Supprimer le vaisseau de la map spaceshipMap
            spaceshipMap.erase(spaceshipName);
        }
        return true;
    }
}

bool Controller::deletePlanet(const string& name) {
    // Rechercher la planète dans la map planetMap
    auto it = planetMap.find(name);
    if (it == planetMap.end()) {
        return false;
    }
    else {
        string planetName = it->first;
        if (planetMap[planetName]) {
            // Parcourir les résidents de la planète
            for (auto resident : it->second->getResident()) {
                if (auto character = dynamic_pointer_cast<Character>(resident.lock())) {
                    // Vérifier si c'est un ennemi
                    if (auto enemy = dynamic_pointer_cast<Enemy>(character)) {
                        cout << "Suppression de l'ennemi : " << enemy->getName() << endl;
                        enemyMap.erase(enemy->getName());
                    }
                    else {
                        // Supprimer le personnage de la map characterMap
                        cout << "Suppression du personnage: " << character->getName() << endl;
                        characterMap.erase(character->getName());
                    }
                }
            }
            // Supprimer la planète de la map planetMap
            planetMap.erase(planetName);
        }
        return true;
    }
}


bool Controller::deleteQuest(const std::string &name) {
    //Rechercher la mission dans la map
    auto it = questMap.find(name);
    if (it == questMap.end()) {
        return false;
    } else {
        string questName = it->first;
        // Supprimer la mission de la map
        questMap.erase(questName);
        return true;
    }
#ifdef DEBUG
    for (auto &pair: questMap) {
        auto &mi = pair.second;
        cout << "Nom : " << mi->getName() << endl;
    }
#endif
}

bool Controller::neutralAttack(string assailant, string defender) {

    auto as = setupRole(assailant,defender)[0];
    auto def = setupRole(assailant,defender)[1];

    int damage = as->getAttackPower();
    int defHealth = def->getHealth();
    int defArmor = def->getArmorPower();
    if (defArmor >= damage)
        def->setArmorPower(defArmor - damage);
    else
        def->setHealth(defHealth + defArmor - damage);

    if (def->getHealth() <= 0) {
        deleteCharacter(defender);
        return true;
    }
    return false;
}

vector<shared_ptr<Character>> Controller::setupRole(string assailant, string defender) {
    vector<shared_ptr<Character>> roles;
    auto it = characterMap.find(assailant);
    auto it2 = enemyMap.find(assailant);
    if (it != characterMap.end()) {
        roles.push_back(it->second);
        auto def = enemyMap.find(defender);
        roles.push_back(def->second);
    }
    else if(it2 != enemyMap.end()) {
        roles.push_back(it2->second);
        auto def = characterMap.find(defender);
        roles.push_back(def->second);
    }
    return roles;
}

Controller::~Controller() {

}