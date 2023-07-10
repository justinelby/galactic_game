//
// Created by justi on 23/06/2023.
//

#ifndef JEU_PERSONNAGES_CHARACTER_H
#define JEU_PERSONNAGES_CHARACTER_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <sstream>

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

protected:
    int ap;
    int dp;
private:
    string name;
    string descr;
    int hp;
    string placeType;
    string place;
};

#endif //JEU_PERSONNAGES_CHARACTER_H
