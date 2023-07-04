//
// Created by justi on 27/06/2023.
//

#ifndef JEU_PERSONNAGES_CONTROLLER_H
#define JEU_PERSONNAGES_CONTROLLER_H
#include <map>
#include "Mission.h"
#include "Character.h"
#include "Planet.h"
#include "Spaceship.h"

using namespace std;


class Controller {
public:
    Controller(string& loadedFile, string& savedFile);

    //Getters et Setters
    vector<shared_ptr<Mission>> getMission();
    map<string, shared_ptr<Character>> getCharacter();
    vector<shared_ptr<Planet>> getPlanet();
    vector<shared_ptr<Spaceship>> getSpaceship();

    //Méthodes
    void loadGame();
    void saveGame();

    void addCharacter(const shared_ptr<Character>&);
    void deleteCharacter(string);
    void addSpaceship(const shared_ptr<Spaceship>&);
    void deleteSpaceship(int);

    string characterToString();
    string spaceshipToString();
    string planetToString();
    string missionToString();
    virtual ~Controller();

private :
    //Attributs
    vector<shared_ptr<Mission>> mission;
    //vector<shared_ptr<Character>> character;
    map<string, shared_ptr<Character>> characterMap;
    vector<shared_ptr<Planet>> planet;
    vector<shared_ptr<Spaceship>> spaceship;
    string loadedFile;
    string savedFile;
};


#endif //JEU_PERSONNAGES_CONTROLLER_H
