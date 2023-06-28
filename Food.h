//
// Created by Mathieu on 28/06/2023.
//

#ifndef JEU_PERSONNAGES_FOOD_H
#define JEU_PERSONNAGES_FOOD_H

#include <iostream>
#include <string>
using namespace std;

class Food {

public:
    Food(string name, string descr, int fullness);
    const string &getDescr() ;
    const string &getName() ;
    int getFullness() const;

private:
    string name;
    string descr;
    int fullness;
};


#endif //JEU_PERSONNAGES_FOOD_H
