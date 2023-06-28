//
// Created by justi on 26/06/2023.
//

#include <fstream>
#include <sstream>
#include "Planet.h"
#include "Character.h"

using namespace std;

Planet::Planet(string nom, string description): name(nom), description(description)
{
}

Planet::Planet(istringstream& iss){
    iss >> name >> description;
}

string Planet::getName() const {
    return name;
}

string Planet::getDescription() const {
    return description;
}

void Planet::addNewPlanetResident(const Character &character) {
    resident.push_back(character);
}

vector<Character> &Planet::getResident() {
    return resident;
}

void Planet::setResident(const Character &resident) {
    this->resident.push_back(resident);
}

Planet::~Planet() {}
