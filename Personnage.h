//
// Created by justi on 23/06/2023.
//

#ifndef JEU_PERSONNAGES_PERSONNAGE_H
#define JEU_PERSONNAGES_PERSONNAGE_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Personnage
{
public:
    //Constructeur/Destructeur
    Personnage(string, string, int, int, string, string);
    Personnage(istringstream&);
    ~Personnage();

    //Accesseurs et mutateurs
    string getNom() const;
    int getPuissance() const ;
    int getSante() const;
    string getPoste() const;
    string getTypeLieu() const;
    int set_sante(int);
    string getLieu() const;
    void setLieu(const string &lieu);

    //Methodes
    void attaque(Personnage &);

private:
    string name;
    string poste;
    int sante;
    int puissance_dattaque;
    string typeLieu;
    string lieu;
};

#endif //JEU_PERSONNAGES_PERSONNAGE_H
