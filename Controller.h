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
    vector<Mission*> getMission();
    vector<Character*> getCharacter();
    vector<Planet*> getPlanet();
    vector<Spaceship*> getSpaceship();

    //Méthodes
    void loadGame();

    virtual ~Controller();

private :
    //Attributs
    vector<Mission*> mission;
    vector<Character*> character;
    vector<Planet*> planet;
    vector<Spaceship*> spaceship;
    string filepath;
};


#endif //JEU_PERSONNAGES_CONTROLLER_H
