//
// Created by justi on 23/06/2023.
//

#ifndef JEU_PERSONNAGES_SPACESHIP_H
#define JEU_PERSONNAGES_SPACESHIP_H
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include "Character.h"
#include "Enemy.h"

using namespace std;

class Spaceship {

public:
    //Constructeur/Destructeur
    Spaceship(string);
    Spaceship(istringstream&);
    virtual ~Spaceship();

    //Accesseurs et mutateurs
    string getName() const;
    vector<weak_ptr<Character>>& getCrew();
    void showCrew();

    //Methodes
    void addCrewMember(shared_ptr<Character>&);
    void addEnemyCrewMember(shared_ptr<Enemy>&);
    void killCrewMember(shared_ptr<Character>&);

private:
    string name;
    vector<weak_ptr<Character>> crew;
};

#endif //JEU_PERSONNAGES_SPACESHIP_H
