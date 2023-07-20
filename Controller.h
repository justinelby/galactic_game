//
// Created by justi on 27/06/2023.
//

#ifndef JEU_PERSONNAGES_CONTROLLER_H
#define JEU_PERSONNAGES_CONTROLLER_H
#include <map>
#include "Quest.h"
#include "Character.h"
#include "Enemy.h"
#include "Planet.h"
#include "Spaceship.h"
#include "Item.h"

using namespace std;


class Controller {
public:
    Controller(string& loadedFile, string& savedFile);


    //Getters & Setters
    map<string, shared_ptr<Quest>> getQuest();
    map<string, shared_ptr<Character>> getCharacter();
    map<string, shared_ptr<Enemy>> getEnemy();
    map<string, shared_ptr<Planet>> getPlanet();
    map<string, shared_ptr<Spaceship>> getSpaceship();
    map<string, unique_ptr<Item>>& getInventory();

    //Methods
    void loadGame();
    void saveGame();

    void addCharacter(const shared_ptr<Character>&);
    void addEnemy(const shared_ptr<Enemy>&);
    void addSpaceship(const shared_ptr<Spaceship>&);
    void addPlanet(const shared_ptr<Planet>&);
    void addQuest(const shared_ptr<Quest>&);
    void addToGameInventory(unique_ptr<Item>&);
    void addToCharacterInventory(string, string);
    bool deleteCharacter(const string& name);
    bool deleteSpaceship(const string& name);
    bool deletePlanet(const string& name);
    bool deleteQuest(const string& name);
    void cleanWeakPtr(vector<weak_ptr<Character>>& vec);
//    void cleanUniquePtr(map<string, unique_ptr<Item>>&);


    bool neutralAttack(string, string);  // returns true if enemy dies
    void looting(string, string);
    void useItem(string, string);
    string characterToString();
    string spaceshipToString();
    string planetToString();
    string questToString();
    string itemToString();
    virtual ~Controller();
private :
    //Attributs
    map<string, shared_ptr<Character>> characterMap;
    map<string, shared_ptr<Enemy>> enemyMap;
    map<string, shared_ptr<Spaceship>> spaceshipMap;
    map<string, shared_ptr<Planet>> planetMap;
    map<string, shared_ptr<Quest>> questMap;
    map<string, unique_ptr<Item>> inventory;
    string loadedFile;

    string savedFile;
    // Methods
    vector<shared_ptr<Character>> setupRole(string, string);
    bool isReplacing();
};


#endif //JEU_PERSONNAGES_CONTROLLER_H
