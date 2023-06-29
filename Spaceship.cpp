//
// Created by justi on 23/06/2023.
//

#include "Spaceship.h"
#include "Character.h"

Spaceship::Spaceship(string nom) {
    this->name = nom;
}

Spaceship::Spaceship(istringstream& iss){
    iss >> name;
}

string Spaceship::getName() const {
    return this->name;
}
vector<weak_ptr<Character>> Spaceship::getCrew() {
    return this->crew;
}

void Spaceship::addCrewMember(shared_ptr<Character>& member) {
    crew.push_back(member);
}

Spaceship::~Spaceship() {

}

