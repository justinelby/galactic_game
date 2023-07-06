//
// Created by justi on 27/06/2023.
//

#ifndef JEU_PERSONNAGES_MISSION_H
#define JEU_PERSONNAGES_MISSION_H
#include <string>
#include <vector>
#include "Quest.h"
#include <fstream>
#include <sstream>

using namespace std;

//Affichage du menu principal
class Quest {

public:
    //Constructeur
    Quest(string nom, string description);
    Quest(istringstream&);
    virtual ~Quest();

    //Accesseurs et mutateurs
    string getName() const;
    string getDescription() const;
    bool getIsCompleted() const;

    //Methodes
    void setIsCompleted(bool completed);

private:
    //Attributs
    string name;
    string description;
    bool isCompleted;
};



#endif //JEU_PERSONNAGES_MISSION_H
