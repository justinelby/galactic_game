//
// Created by justi on 26/06/2023.
//

#ifndef JEU_PERSONNAGES_PLANET_H
#define JEU_PERSONNAGES_PLANET_H
#include <string>
#include <vector>
#include "Character.h"
#include <fstream>
#include <sstream>

using namespace std;


class Planet {
public:
    Planet(string nom, string description, vector<Character*>);
    Planet(istringstream&);
    string getName() const;
    string getDescription() const;
    vector<Character*> getResident();

    void setResident(Character* resident);
    void addNewPlanetResident(Character*);

    virtual ~Planet();

private:
    string name;
    string description;
    vector<Character*> resident;
};


#endif //JEU_PERSONNAGES_PLANET_H
