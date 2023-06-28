//
// Created by justi on 27/06/2023.
//

#ifndef JEU_PERSONNAGES_CONTROLEUR_H
#define JEU_PERSONNAGES_CONTROLEUR_H

#include "Mission.h"
#include "Personnage.h"
#include "Planete.h"
#include "Vaisseau.h"

using namespace std;


class Controleur {
public:
    Controleur(string& filepath);

    //Getters et Setters
    vector<Mission> &getMission();
    vector<Personnage> &getPersonnage();
    vector<Planete> &getPlanete();
    vector<Vaisseau> &getVaisseau();

    //Méthodes
    void loadGame();

    virtual ~Controleur();

private :
    //Attributs
    vector<Mission> mission;
    vector<Personnage> personnage;
    vector<Planete> planete;
    vector<Vaisseau> vaisseau;
    string filepath;
};


#endif //JEU_PERSONNAGES_CONTROLEUR_H
