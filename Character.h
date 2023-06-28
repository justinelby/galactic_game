//
// Created by justi on 23/06/2023.
//

#ifndef JEU_PERSONNAGES_CHARACTER_H
#define JEU_PERSONNAGES_CHARACTER_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Character
{
public:
    //Constructeur/Destructeur
    Character(string, string, int, int, string, string);
    Character(istringstream&);
    ~Character();

    //Accesseurs et mutateurs
    string getName() const;
    int getAttackPower() const ;
    int getHealth() const;
    string getPoste() const;
    string getPlaceType() const;
    int setHealth(int);
    string getPlace() const;
    void setPlace(const string &place);

    //Methodes
    void attaque(Character &);

private:
    string name;
    string poste;
    int health;
    int attackPower;
    string placeType;
    string place;
};

#endif //JEU_PERSONNAGES_CHARACTER_H
