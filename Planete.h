//
// Created by justi on 26/06/2023.
//

#ifndef JEU_PERSONNAGES_PLANETE_H
#define JEU_PERSONNAGES_PLANETE_H
#include <string>
#include <vector>
#include "Personnage.h"
#include <fstream>
#include <sstream>

using namespace std;


class Planete {
public:
    Planete(string nom, string description);
    Planete(istringstream&);
    string getNom() const;
    string getDescription() const;
    vector<Personnage>& getHabitants();

    void setHabitants(const Personnage &habitants);
    void ajouterHabitantPlanete(const Personnage&);

    virtual ~Planete();

private:
    string nom;
    string description;
    vector<Personnage> habitants;

};


#endif //JEU_PERSONNAGES_PLANETE_H
