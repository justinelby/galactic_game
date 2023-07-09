//
// Created by justi on 27/06/2023.
//

#ifndef JEU_PERSONNAGES_CONTROLLER_H
#define JEU_PERSONNAGES_CONTROLLER_H
#include <map>
#include <ctime>
#include "Quest.h"
#include "Character.h"
#include "Enemy.h"
#include "Planet.h"
#include "Spaceship.h"

using namespace std;


class Controller {
public:
    Controller(string& loadedFile, string& savedFile);

    //Getters et Setters
    map<string, shared_ptr<Quest>> getQuest();
    map<string, shared_ptr<Character>> getCharacter();
    map<string, shared_ptr<Enemy>> getEnemy();
    map<string, shared_ptr<Planet>> getPlanet();
    map<string, shared_ptr<Spaceship>> getSpaceship();

    //Méthodes
    void loadGame();
    void saveGame();

    void addCharacter(const shared_ptr<Character>&);
    void addEnemy(const shared_ptr<Enemy>&);
    void addSpaceship(const shared_ptr<Spaceship>&);
    void addPlanet(const shared_ptr<Planet>&);
    void addQuest(const shared_ptr<Quest>&);
    bool deleteCharacter(const string& name);
    bool deleteSpaceship(const string& name);
    bool deletePlanet(const string& name);
    bool deleteQuest(const string& name);
    void cleanWeakPtr(vector<weak_ptr<Character>>& vec);
    void characterAttackEnemy(Character& character, Enemy& enemy);
    void enemyAttackCharacter(Enemy& enemy, Character& character);

    string characterToString();
    string spaceshipToString();
    string planetToString();
    string questToString();
    virtual ~Controller();

private :
    //Attributs
    map<string, shared_ptr<Character>> characterMap;
    map<string, shared_ptr<Enemy>> enemyMap;
    map<string, shared_ptr<Spaceship>> spaceshipMap;
    map<string, shared_ptr<Planet>> planetMap;
    map<string, shared_ptr<Quest>> questMap;
    string loadedFile;
    string savedFile;
};


#endif //JEU_PERSONNAGES_CONTROLLER_H
