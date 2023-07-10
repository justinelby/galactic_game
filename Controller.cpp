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

using namespace std;

Controller::Controller(string &loadedFile, string &savedFile) : loadedFile(loadedFile), savedFile(savedFile) {
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
    ifstream file(loadedFile);

    if (!file.is_open()) {
        cout << "Le fichier ne s'est pas ouvert" << endl;
    }
    string line;
    vector<shared_ptr<Character>> crew, resident;

    //On parcourt les lignes du fichier
    while (getline(file, line)) {
        istringstream iss(line);
        string type;
        getline(iss, type, ';');

        //Si la ligne commence par character, on récupère les informations associées
        if (type == "Character" || type == "Enemy") {
            string name;
            getline(iss, name, ';');

            string poste;
            getline(iss, poste, ';');

            string health;
            getline(iss, health, ';');

            string attackPower;
            getline(iss, attackPower, ';');

            string armorPower;
            getline(iss, armorPower, ';');

            string placeType;
            getline(iss, placeType, ';');

            string place;
            getline(iss, place);

            if (type == "Character") {
                auto newCharacter = make_shared<Character>(name, poste, stoi(health), stoi(attackPower),
                                                           stoi(armorPower), placeType, place);
                addCharacter(newCharacter);
            } else {    // if Enemy
                auto newEnemy = make_shared<Enemy>(name, poste, stoi(health), stoi(attackPower), stoi(armorPower),
                                                   placeType, place);
                addEnemy(newEnemy);
            }


        } else if (type ==
                   "Spaceship") //Si la ligne commence par spaceship, on récupère les informations associées et on les stocke
        {
            string name;
            getline(iss, name, ';');

            auto newSpaceship = make_shared<Spaceship>(name);
            addSpaceship(newSpaceship);
        } else if (type ==
                   "Planet") //Si la ligne commence par planet, on récupère les informations associées et on les stocke
        {
            string name;
            getline(iss, name, ';');

            string description;
            getline(iss, description, '\n');

            auto newPlanet = make_shared<Planet>(name, description);
            addPlanet(newPlanet);
        } else if (type ==
                   "Quest")//Si la ligne commence par mission, on récupère les informations associées et on les stocke
        {
            string name;
            getline(iss, name, ';');

            string description;
            getline(iss, description, '\n');
            iss >> description;

            auto newMission = make_shared<Quest>(name, description);
            addQuest(newMission);
        }
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

bool Controller::deleteCharacter(const string &name) {
    //Rechercher le personnage dans la map
    auto it = characterMap.find(name);
    auto it2 = enemyMap.find(name);

    if (it != characterMap.end()) {
        string characterName = it->first;
        // Vérifier si le pointeur de personnage est nul
        if (characterMap[characterName]) {
            //Obtenir le type de lieu où se situe le personnage
            string typePlace = characterMap[characterName]->getPlaceType();
            //Si le personnage est sur une planete
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
                //Si le personnage est sur un vaisseau
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
    } else if (it2 != enemyMap.end()) {
        string enemyName = it2->first;
        // Vérifier si le pointeur de personnage est nul
        if (enemyMap[enemyName]) {
            //Obtenir le type de lieu où se situe le personnage
            string typePlace = enemyMap[enemyName]->getPlaceType();
            //Si le personnage est sur une planete
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
                //Si le personnage est sur un vaisseau
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
    } else {
        return false;
    }
}

bool Controller::deleteSpaceship(const string &name) {
    //Rechercher le vaisseau dans la map
    auto it = spaceshipMap.find(name);
    if (it == spaceshipMap.end()) {
        return false;
    } else {
        string spaceshipName = it->first;
        if (spaceshipMap[spaceshipName]) {
            // Parcourir les membres de l'équipage du vaisseau
            for (auto member: it->second->getCrew()) {
                if (member.lock()) {
                    if (auto character = dynamic_pointer_cast<Character>(member.lock())) {
                        // Vérifier si c'est un ennemi
                        if (auto enemy = dynamic_pointer_cast<Enemy>(character)) {
                            cout << "Suppression de l'ennemi : " << enemy->getName() << endl;
                            enemyMap.erase(enemy->getName());
                        } else {
                            // Supprimer le personnage de la map characterMap
                            cout << "Suppression du personnage: " << character->getName() << endl;
                            characterMap.erase(character->getName());
                        }
                    }
                }
            }
            // Supprimer la planète de la map planetMap
            spaceshipMap.erase(spaceshipName);
        }
        return true;
    }
#ifdef DEBUG
    for (auto &pair: spaceshipMap) {
        auto &ship = pair.second;
        cout << "Nom : " << ship->getName() << endl;
    }
#endif
}

bool Controller::deletePlanet(const string &name) {
    // Rechercher la planète dans la map
    auto it = planetMap.find(name);
    if (it == planetMap.end()) {
        return false;
    } else {
        string planetName = it->first;
        if (planetMap[planetName]) {
            // Parcourir les résidents de la planète
            for (auto resident: it->second->getResident()) {
                if (resident.lock()) {
                    if (auto character = dynamic_pointer_cast<Character>(resident.lock())) {
                        // Vérifier si c'est un ennemi
                        if (auto enemy = dynamic_pointer_cast<Enemy>(character)) {
                            cout << "Suppression de l'ennemi : " << enemy->getName() << endl;
                            enemyMap.erase(enemy->getName());
                        } else {
                            // Supprimer le personnage de la map characterMap
                            cout << "Suppression du personnage: " << character->getName() << endl;
                            characterMap.erase(character->getName());
                        }
                    }
                }
            }
            // Supprimer la planète de la map planetMap
            planetMap.erase(planetName);
        }
        return true;
    }
#ifdef DEBUG
    for (auto &pair: planetMap) {
        auto &pla = pair.second;
        cout << "Nom : " << pla->getName() << endl;
    }
#endif
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

void Controller::characterAttackEnemy(Character &character, Enemy &enemy) {
    cout << character.getName() << " attaque " << enemy.getName() << " !" << endl;
    // Vérifier que le personnage attaque un ennemi et que l'ennemi attaque un personnage
    if (dynamic_cast<Character *>(&character) && dynamic_cast<Enemy *>(&enemy)) {
        // Réduire la santé de l'ennemi en fonction de la puissance d'attaque du personnage
        int damage = character.getAttackPower();
        int enemyHealth = enemy.getHealth();
        int enemyArmor = enemy.getArmorPower();
        if (enemyArmor >= damage)
            enemy.setArmorPower(enemyArmor - damage);
        else { enemy.setHealth(enemyHealth + enemyArmor - damage);}

        // Vérifier si l'ennemi est vaincu
        if (enemy.getHealth() <= 0) {
            // Supprimer l'ennemi de la map des ennemis du contrôleur
            cout << enemy.getName() << " a ete execute." << endl;
            deleteCharacter(enemy.getName());
        }
    } else {
        cout << "Invalid attack! Characters can only attack enemies." << endl;
    }
}

void Controller::enemyAttackCharacter(Enemy &enemy, Character &character) {
    cout << enemy.getName() << " attaque " << character.getName()<< " !" << endl;
    // Vérifier que le personnage attaque un ennemi et que l'ennemi attaque un personnage
    if (dynamic_cast<Enemy *>(&enemy) && dynamic_cast<Character *>(&character)) {
        // Réduire la santé de l'ennemi en fonction de la puissance d'attaque du personnage
        int damage = enemy.getAttackPower();
        int characterHealth = character.getHealth();
        int characterArmor = character.getArmorPower();
        if (characterArmor >= damage)
            character.setArmorPower(characterArmor - damage);
        else { character.setHealth(characterHealth + characterArmor - damage);}
        character.setHealth(characterHealth - damage);

        // Vérifier si le personnage est vaincu
        if (character.getHealth() <= 0) {
            // Supprimer le personnage de la map des personnages du contrôleur
            cout << character.getName() << "a ete execute." << endl;
            deleteCharacter(character.getName());
        }
    } else {
        cout << "Invalid attack! Enemies can only attack characters." << endl;
    }
}


Controller::~Controller() {

}