//
// Created by justi on 23/06/2023.
//

#ifndef JEU_PERSONNAGES_CHARACTER_H
#define JEU_PERSONNAGES_CHARACTER_H
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Character
{
public:
    // Constructor / Destructor
    Character(string, string, int, int,int, string, string);
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
    string getPlaceType() const;
    void setPlaceType(const string &place);
    string getPlace() const;
    void setPlace(const string &place);

protected:
    int attackPower;
    int armorPower;
private:
    string name;
    string descr;
    int health;
    string placeType;
    string place;
};

#endif //JEU_PERSONNAGES_CHARACTER_H
