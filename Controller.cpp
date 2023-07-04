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

Controller::Controller(string& loadedFile, string& savedFile) : loadedFile(loadedFile), savedFile(savedFile){
}

vector<shared_ptr<Mission>> Controller::getMission(){
    return mission;
}

map<string, shared_ptr<Character>> Controller::getCharacter(){
    return characterMap;
}

vector<shared_ptr<Planet>> Controller::getPlanet(){
    return planet;
}

vector<shared_ptr<Spaceship>> Controller::getSpaceship() {
    return spaceship;
}

void Controller::loadGame() {
    ifstream file(loadedFile);

    if(!file.is_open())
    {
        cout << "Le fichier ne s'est pas ouvert" << endl;
    }
    string line;
    vector<shared_ptr<Character>> crew, resident;

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

            auto newCharacter = make_shared<Character>(name, poste, stoi(health), stoi(attackPower), placeType, place);
            addCharacter(newCharacter);

            //Ajout du personnage à l'équipage du vaisseau auquel il est associé
            for(auto& ship : spaceship)
            {
                if(place==ship->getName()){
                    ship->addCrewMember(characterMap[name]);
                    break;
                }
            }
            for(auto& pla : planet)
            {
                if(place==pla->getName()){
                    pla->addNewPlanetResident(characterMap[name]);
                    break;
                }
            }
        }
        else if (type == "Spaceship") //Si la ligne commence par spaceship, on récupère les informations associées et on les stocke
        {
            string name;
            getline(iss, name, ';');

            auto newSpaceship = make_shared<Spaceship>(name);
            addSpaceship(newSpaceship);
        }
        else if (type == "Planet") //Si la ligne commence par planet, on récupère les informations associées et on les stocke
        {
            string name;
            getline(iss, name, ';');

            string description;
            getline(iss, description, '\n');

            auto newPlanet = make_shared<Planet>(name, description);
            planet.push_back(newPlanet);
        }
        else if (type == "Mission")//Si la ligne commence par mission, on récupère les informations associées et on les stocke
        {
            string name;
            getline(iss, name, ';');

            string description;
            getline(iss, description, '\n');
            iss >> description;

            auto newMission = make_shared<Mission>(name, description);
            mission.push_back(newMission);
        }
    }
}

string Controller::planetToString(){
    ostringstream oss;
    for (const auto& p : planet) {
        oss << "Planet;" << p->getName()<<";"<<p->getDescription()<<"\n";
    }
    return oss.str();
}
string Controller::spaceshipToString(){
    ostringstream oss;
    for (const auto& s : spaceship) {
        oss << "Spaceship;" << s->getName()<<"\n";
    }
    return oss.str();
}

string Controller::characterToString(){
    ostringstream oss;
    for (const auto& pair : characterMap) {
        auto c = pair.second;
        oss << "Character;" << c->getName()<<";"<<c->getPoste()<<";"<<c->getHealth()<<";"<<c->getAttackPower()<<";"<<c->getPlaceType()<<";"<<c->getPlace()<<"\n";
    }
    return oss.str();
}

string Controller::missionToString(){
    ostringstream oss;
    for (const auto& m : mission) {
        oss << "Mission;" << m->getName()<<";"<<m->getDescription()<<"\n";
    }
    return oss.str();
}

void Controller::saveGame(){
    //Ecriture du fichier de sauvegarde
    ofstream file(savedFile);
    file << planetToString()<< spaceshipToString() << characterToString() << missionToString() << endl;
}
void Controller::addCharacter(const shared_ptr<Character>& newCharacter) {
    characterMap[newCharacter->getName()] = newCharacter;
}


void Controller::deleteCharacter(string name) {
 auto it = characterMap.find(name);
    characterMap.erase(it);
}


void Controller::addSpaceship(const shared_ptr<Spaceship>& newSpaceship) {
    spaceship.push_back(newSpaceship);
}

void Controller::deleteSpaceship(int index) {
    spaceship.erase(spaceship.begin() + index);
}


Controller::~Controller()
{

}