//
// Created by justi on 23/06/2023.
//

#include "Vaisseau.h"
#include "Personnage.h"

Vaisseau::Vaisseau(string nom) {
    this->nom = nom;
}

Vaisseau::Vaisseau(istringstream& iss){
    iss >> nom;
}

string Vaisseau::getNom() const {
    return this->nom;
}
vector<Personnage>& Vaisseau::get_equipage() {
    return this->equipage;
}

void Vaisseau::ajouterMembreEquipage(const Personnage &membre) {
    equipage.push_back(membre);
}

Vaisseau::~Vaisseau() {

}

