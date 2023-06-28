//
// Created by justi on 26/06/2023.
//

#include <fstream>
#include <sstream>
#include "Planete.h"
#include "Personnage.h"

using namespace std;

Planete::Planete(string nom, string description): nom(nom), description(description)
{
}

Planete::Planete(istringstream& iss){
    iss >> nom >> description;
}


string Planete::getNom() const {
    return nom;
}

string Planete::getDescription() const {
    return description;
}

void Planete::ajouterHabitantPlanete(const Personnage &personne) {
    habitants.push_back(personne);
}

vector<Personnage> &Planete::getHabitants() {
    return habitants;
}

void Planete::setHabitants(const Personnage &habitants) {
    this->habitants.push_back(habitants);
}

Planete::~Planete() {}
