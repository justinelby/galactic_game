//
// Created by justi on 27/06/2023.
//

#include "Controleur.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "Personnage.h"
#include "Vaisseau.h"
#include "Planete.h"

Controleur::Controleur(string& filepath) : filepath(filepath){
}

vector<Mission> &Controleur::getMission(){
    return mission;
}

vector<Personnage> &Controleur::getPersonnage(){
    return personnage;
}

vector<Planete> &Controleur::getPlanete(){
    return planete;
}

vector<Vaisseau> &Controleur::getVaisseau() {
    return vaisseau;
}

void Controleur::loadGame() {
    ifstream file(filepath);

    if(!file.is_open())
    {
        cout << "Le fichier ne s'est pas ouvert" << endl;
    }
    string line;

    //On parcourt les lignes du fichier
    while(getline(file, line)){
        istringstream iss(line);
        string type;
        getline(iss, type, ';');

        //Si la ligne commence par personnage, on récupère les informations associées
        if (type == "Personnage")
        {
            string nom;
            getline(iss, nom, ';');

            string poste;
            getline(iss, poste, ';');

            string sante;
            getline(iss, sante, ';');

            string puissance_dattaque;
            getline(iss, puissance_dattaque, ';');

            string typeLieu;
            getline(iss, typeLieu, ';');

            string lieu;
            getline(iss, lieu);

            personnage.push_back(Personnage(nom, poste, stoi(sante), stoi(puissance_dattaque), typeLieu, lieu));
        }
        else if (type == "Vaisseau") //Si la ligne commence par vaisseau, on récupère les informations associées et on les stocke
        {
            string nom;
            getline(iss, nom, ';');
            vaisseau.push_back(Vaisseau(nom));
        }
        else if (type == "Planete") //Si la ligne commence par planete, on récupère les informations associées et on les stocke
        {
            string nom;
            getline(iss, nom, ';');

            string description;
            getline(iss, description, '\n');
            iss >> description;

            planete.push_back(Planete(nom, description));
        }
        else if (type == "Mission")//Si la ligne commence par mission, on récupère les informations associées et on les stocke
        {
            string nom;
            getline(iss, nom, ';');

            string description;
            getline(iss, description, '\n');
            iss >> description;

            mission.push_back(Mission(nom, description));
        }
    }
}

Controleur::~Controleur()
{

}