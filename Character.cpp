#include "Character.h"
#include "Quest.h"
#include <iostream>
#include <string>
#include <sstream>


Character::Character(string name, string desc, int hp, int ap, int dp, string placeType, string place)
{
    this->name = name;
    this->descr = desc;
    this->health = hp;
    this->attackPower = ap;
    this->armorPower = dp;
    this->placeType = placeType;
    this->place = place;
}

Character::Character(istringstream& iss) {
    iss >> name >> descr >> health >> attackPower >> armorPower >> placeType >> place;
}

string Character::getName() const { return name; }
string Character::getDescr() const { return descr; }

int Character::getHealth() const { return health; }
void Character::setHealth(int const hp) { this->health = hp; }

int Character::getAttackPower() const { return attackPower; }
void Character::setAttackPower(int ap) { this->attackPower = ap; }

int Character::getArmorPower() const { return armorPower; }
void Character::setArmorPower(int dp) { this->armorPower = dp; }

string Character::getPlaceType() const { return placeType; }
void Character::setPlaceType(const string &pt) { this->place = pt; }    // add verification if input is valid

string Character::getPlace() const { return place; }
void Character::setPlace(const string &pl) { this->place = pl; }    // add verification if input is valid



void Character::attack(Character &personnageCible){
    std::cout << name << " attaque " << personnageCible.getName() << " !" << std::endl;
    personnageCible.setHealth(personnageCible.getHealth() - this->attackPower);
    //std::cout <<"Vies restantes a " << personnageCible.getName() << " : " << personnageCible.getHealth() << std::endl;
}

Character::~Character()
{
    //dtor
}