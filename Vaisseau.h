//
// Created by justi on 23/06/2023.
//

#ifndef JEU_PERSONNAGES_VAISSEAU_H
#define JEU_PERSONNAGES_VAISSEAU_H
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Personnage.h"

using namespace std;

class Vaisseau {

public:
    //Constructeur/Destructeur
    Vaisseau(string);
    Vaisseau(istringstream&);
    virtual ~Vaisseau();

    //Accesseurs et mutateurs
    string getNom() const;
    vector<Personnage>& get_equipage();

    //Methodes
    void ajouterMembreEquipage(const Personnage &);

private:
    string nom;
    vector<Personnage> equipage;
};

#endif //JEU_PERSONNAGES_VAISSEAU_H
