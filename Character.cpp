#include "Character.h"
#include "Quest.h"
#include <iostream>
#include <string>
#include <sstream>


Character::Character(string name, string desc, int hp, int ap, int dp, string placeType, string place)
{
    this->name = name;
    this->descr = desc;
    this->hp = hp;
    this->ap = ap;
    this->dp = dp;
    this->placeType = placeType;
    this->place = place;
}

Character::Character(istringstream& iss) {
    iss >> name >> descr >> hp >> ap >> dp >> placeType >> place;
}

string Character::getName() const { return name; }
string Character::getDescr() const { return descr; }

int Character::getHealth() const { return hp; }
void Character::setHealth(int const hp) { this->hp = hp; }

int Character::getAttackPower() const { return ap; }
void Character::setAttackPower(int ap) { this->ap = ap; }

int Character::getArmorPower() const { return dp; }
void Character::setArmorPower(int dp) { this->dp = dp; }

vector<int> Character::getStatus() { return {hp, ap, dp}; }

string Character::getPlaceType() const { return placeType; }
void Character::setPlaceType(const string &pt) { this->place = pt; }    // add verification if input is valid

string Character::getPlace() const { return place; }
void Character::setPlace(const string &pl) { this->place = pl; }    // add verification if input is valid

Character::~Character()
{
    //dtor
}