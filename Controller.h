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
    map<string, shared_ptr<Mission>> getMission();
    map<string, shared_ptr<Character>> getCharacter();
    map<string, shared_ptr<Planet>> getPlanet();
    map<string, shared_ptr<Spaceship>> getSpaceship();

    //Méthodes
    void loadGame();
    void saveGame();

    void addCharacter(const shared_ptr<Character>&);
    void addSpaceship(const shared_ptr<Spaceship>&);
    void addPlanet(const shared_ptr<Planet>&);
    void addMission(const shared_ptr<Mission>&);
    bool deleteCharacter(const string& name);
    void deleteSpaceship(const string& name);
    void deletePlanet(const string& name);
    void deleteMission(const string& name);

    void deleteSpaceship(int);

    string characterToString();
    string spaceshipToString();
    string planetToString();
    string missionToString();
    virtual ~Controller();

private :
    //Attributs
    map<string, shared_ptr<Character>> characterMap;
    map<string, shared_ptr<Spaceship>> spaceshipMap;
    map<string, shared_ptr<Planet>> planetMap;
    map<string, shared_ptr<Mission>> missionMap;
    string loadedFile;
    string savedFile;
};


#endif //JEU_PERSONNAGES_CONTROLLER_H
