//
// Created by justi on 26/06/2023.
//

#include <sstream>
#include <algorithm>
#include "Planet.h"
#include "Character.h"

using namespace std;

Planet::Planet(string nom, string description): name(nom), description(description) {}

Planet::Planet(istringstream& iss){
    iss >> name >> description;
}

string Planet::getName() const { return name; }


string Planet::getDescription() const {
    return description;
}

void Planet::addNewPlanetResident(shared_ptr<Character>& character) {
    resident.push_back(character);
}
void Planet::addNewPlanetEnemyResident(shared_ptr<Enemy>& enemy) {
    resident.push_back(enemy);
}

vector<weak_ptr<Character>>& Planet::getResident() {
    return resident;
}

void Planet::setResident(shared_ptr<Character>& resident) {
    this->resident.push_back(resident);
}

Planet::~Planet() {}
