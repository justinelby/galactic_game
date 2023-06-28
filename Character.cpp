#include "Character.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


Character::Character(string name, string poste, int sante, int attackPower, string placeType, string place)
{
    this->name = name;
    this->poste = poste;
    this->health=sante;
    this->attackPower = attackPower;
    this->placeType = placeType;
    this->place = place;
}

Character::Character(istringstream& iss) {
    iss >> name >> poste >> health >> attackPower >> placeType >> place;
}

string Character::getName() const
{
    return name;
}

int Character::getHealth() const
{
    return health;
}

int Character::getAttackPower() const
{
    return attackPower;
}

int Character::setHealth(int const health)
{
    this->health=health;
    return health;
}

void Character::attaque(Character &personnageCible){
    std::cout << name << " attaque " << personnageCible.getName() << " !" << std::endl;
    personnageCible.setHealth(personnageCible.getHealth() - this->attackPower);
    //std::cout <<"Vies restantes a " << personnageCible.getName() << " : " << personnageCible.getHealth() << std::endl;
}

Character::~Character()
{
    //dtor
}

string Character::getPoste() const {
    return poste;
}

string Character::getPlaceType() const {
    return placeType;
}

string Character::getPlace() const {
    return place;
}

void Character::setPlace(const string &place) {
    Character::place = place;
}

