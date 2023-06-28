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
#include "Character.h"

using namespace std;

class Spaceship {

public:
    //Constructeur/Destructeur
    Spaceship(string);
    Spaceship(istringstream&);
    virtual ~Spaceship();

    //Accesseurs et mutateurs
    string getName() const;
    vector<Character>& getCrew();

    //Methodes
    void addCrewMember(const Character &);

private:
    string name;
    vector<Character> crew;
};

#endif //JEU_PERSONNAGES_SPACESHIP_H
