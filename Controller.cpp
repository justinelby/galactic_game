//
// Created by justi on 27/06/2023.
//

#include "Controller.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "Character.h"
#include "Spaceship.h"
#include "Planet.h"

Controller::Controller(string& filepath) : filepath(filepath){
}

vector<Mission> &Controller::getMission(){
    return mission;
}

vector<Character> &Controller::getCharacter(){
    return character;
}

vector<Planet> &Controller::getPlanet(){
    return planet;
}

vector<Spaceship> &Controller::getSpaceship() {
    return spaceship;
}

void Controller::loadGame() {
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

        //Si la ligne commence par character, on récupère les informations associées
        if (type == "Character")
        {
            string name;
            getline(iss, name, ';');

            string poste;
            getline(iss, poste, ';');

            string health;
            getline(iss, health, ';');

            string attackPower;
            getline(iss, attackPower, ';');

            string placeType;
            getline(iss, placeType, ';');

            string place;
            getline(iss, place);

            character.push_back(Character(name, poste, stoi(health), stoi(attackPower), placeType, place));
        }
        else if (type == "Spaceship") //Si la ligne commence par spaceship, on récupère les informations associées et on les stocke
        {
            string name;
            getline(iss, name, ';');
            spaceship.push_back(Spaceship(name));
        }
        else if (type == "Planet") //Si la ligne commence par planet, on récupère les informations associées et on les stocke
        {
            string name;
            getline(iss, name, ';');

            string description;
            getline(iss, description, '\n');
            iss >> description;

            planet.push_back(Planet(name, description));
        }
        else if (type == "Mission")//Si la ligne commence par mission, on récupère les informations associées et on les stocke
        {
            string name;
            getline(iss, name, ';');

            string description;
            getline(iss, description, '\n');
            iss >> description;

            mission.push_back(Mission(name, description));
        }
    }
}

Controller::~Controller()
{

}