//
// Created by justi on 27/06/2023.
//

#ifndef JEU_PERSONNAGES_MISSION_H
#define JEU_PERSONNAGES_MISSION_H
#include <string>
#include <vector>
#include "Mission.h"
#include <fstream>
#include <sstream>

using namespace std;

//Affichage du menu principal
class Mission {

public:
    //Constructeur
    Mission(string nom, string description);
    Mission(istringstream&);
    virtual ~Mission();

    //Accesseurs et mutateurs
    string getNom() const;
    string getDescription() const;
    bool getIsCompleted() const;

    //Methodes
    void setIsCompleted(bool completed);

private:
    //Attributs
    string nom;
    string description;
    bool isCompleted;
};



#endif //JEU_PERSONNAGES_MISSION_H
