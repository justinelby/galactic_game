//
// Created by justi on 23/06/2023.
//

#ifndef JEU_PERSONNAGES_CHARACTER_H
#define JEU_PERSONNAGES_CHARACTER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <ctime>
#include <fstream>
#include <sstream>
#include "Item.h"

using namespace std;

class Character
{
public:
    // Constructor / Destructor
    Character(string, string, int, int, int, string, string);
    Character(istringstream&);
    ~Character();

    // Getters & setters
    string getName() const;
    string getDescr() const;
    int getHealth() const;
    void setHealth(int);
    int getAttackPower() const;
    virtual void setAttackPower(int);  // different for Enemy
    int getArmorPower() const;
    void setArmorPower(int);
    vector<int> getStatus();
    string getPlaceType() const;
    void setPlaceType(const string &place);
    string getPlace() const;
    void setPlace(const string &place);
    map<string, unique_ptr<Item>>& getInventory();

protected:
    int ap;
    int dp;
private:
    // Attributes
    string name;
    string descr;
    int hp;
    string placeType;
    string place;
    map<string, unique_ptr<Item>> inventory;

};

#endif //JEU_PERSONNAGES_CHARACTER_H
