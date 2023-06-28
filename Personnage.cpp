#include "Personnage.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


Personnage::Personnage(string name, string poste, int sante, int puissance_dattaque, string typeLieu, string lieu)
{
    this->name = name;
    this->poste = poste;
    this->sante=sante;
    this->puissance_dattaque = puissance_dattaque;
    this->typeLieu = typeLieu;
    this->lieu = lieu;
}

Personnage::Personnage(istringstream& iss) {
    iss >> name >> poste >> sante >> puissance_dattaque >> typeLieu >> lieu;
}

string Personnage::getNom() const
{
    return name;
}

int Personnage::getSante() const
{
    return sante;
}

int Personnage::getPuissance() const
{
    return puissance_dattaque;
}

int Personnage::set_sante(int const sante)
{
    this->sante=sante;
    return sante;
}

void Personnage::attaque(Personnage &personnageCible){
    std::cout << name << " attaque " << personnageCible.getNom() << " !" << std::endl;
    personnageCible.set_sante(personnageCible.getSante() - this->puissance_dattaque);
    //std::cout <<"Vies restantes a " << personnageCible.getNom() << " : " << personnageCible.getSante() << std::endl;
}

Personnage::~Personnage()
{
    //dtor
}

string Personnage::getPoste() const {
    return poste;
}

string Personnage::getTypeLieu() const {
    return typeLieu;
}

string Personnage::getLieu() const {
    return lieu;
}

void Personnage::setLieu(const string &lieu) {
    Personnage::lieu = lieu;
}

