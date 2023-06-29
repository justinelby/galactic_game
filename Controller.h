//
// Created by justi on 27/06/2023.
//

#ifndef JEU_PERSONNAGES_CONTROLLER_H
#define JEU_PERSONNAGES_CONTROLLER_H

#include "Mission.h"
#include "Character.h"
#include "Planet.h"
#include "Spaceship.h"

using namespace std;


class Controller {
public:
    Controller(string& filepath);

    //Getters et Setters
    vector<shared_ptr<Mission>> getMission();
    vector<shared_ptr<Character>> getCharacter();
    vector<shared_ptr<Planet>> getPlanet();
    vector<shared_ptr<Spaceship>> getSpaceship();

    //Méthodes
    void loadGame();

    virtual ~Controller();

private :
    //Attributs
    vector<shared_ptr<Mission>> mission;
    vector<shared_ptr<Character>> character;
    vector<shared_ptr<Planet>> planet;
    vector<shared_ptr<Spaceship>> spaceship;
    string filepath;
};


#endif //JEU_PERSONNAGES_CONTROLLER_H
