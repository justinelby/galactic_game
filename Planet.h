//
// Created by justi on 26/06/2023.
//

#ifndef JEU_PERSONNAGES_PLANET_H
#define JEU_PERSONNAGES_PLANET_H
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
#include "Character.h"
#include "Enemy.h"

using namespace std;


class Planet {
public:
    Planet(string nom, string description);
    Planet(istringstream&);
    string getName() const;
    string getDescription() const;
    vector<weak_ptr<Character>>& getResident();
    void setResident(shared_ptr<Character>&);
    void cleanPlanetResidentWeakPtr();

    void addNewPlanetResident(shared_ptr<Character>&);
    void addNewPlanetEnemyResident(shared_ptr<Enemy>&);
    virtual ~Planet();

private:
    string name;
    string description;
    vector<weak_ptr<Character>> resident;
};


#endif //JEU_PERSONNAGES_PLANET_H
