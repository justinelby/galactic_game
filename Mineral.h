//
// Created by Mathieu on 28/06/2023.
//

#ifndef JEU_PERSONNAGES_MINERAL_H
#define JEU_PERSONNAGES_MINERAL_H

#include <iostream>
#include <string>
using namespace std;

class Mineral {
public:
    Mineral(string name, string descr, int fullness);
    const string &getDescr() ;
    const string &getName() ;
    int getRarity() const;

private:
    string name;
    string descr;
    int rarity;
};



#endif //JEU_PERSONNAGES_MINERAL_H
