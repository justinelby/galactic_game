//
// Created by Mathieu on 20/07/2023.
//

#ifndef JEU_PERSONNAGES_WEAPON_H
#define JEU_PERSONNAGES_WEAPON_H

#include <string>
using namespace std;

class Weapon {
public:
    Weapon(string, string, int);
    string getName() const;
    string getDescription() const;

protected:
    string name, descr;
    int power;
};


#endif //JEU_PERSONNAGES_WEAPON_H
