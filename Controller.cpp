//
// Created by justi on 27/06/2023.
//
#define DEBUG
#include "Controller.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "Character.h"
#include "Spaceship.h"
#include "Planet.h"

using namespace std;

Controller::Controller(string& loadedFile, string& savedFile) : loadedFile(loadedFile), savedFile(savedFile){
}

map<string, shared_ptr<Mission>> Controller::getMission(){
    return missionMap;
}

map<string, shared_ptr<Character>> Controller::getCharacter(){
    return characterMap;
}

map<string, shared_ptr<Planet>> Controller::getPlanet(){
    return planetMap;
}

map<string, shared_ptr<Spaceship>> Controller::getSpaceship() {
    return spaceshipMap;
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
            addPlanet(newPlanet);
        }
        else if (type == "Mission")//Si la ligne commence par mission, on récupère les informations associées et on les stocke
        {
            string name;
            getline(iss, name, ';');

            string description;
            getline(iss, description, '\n');
            iss >> description;

            auto newMission = make_shared<Mission>(name, description);
            addMission(newMission);
        }
    }
}

string Controller::characterToString(){
    ostringstream oss;
    for (const auto& pair : characterMap) {
        auto c = pair.second;
        oss << "Character;" << c->getName()<<";"<< c->getPoste()<<";"<< c->getHealth()<<";"<< c->getAttackPower()<<";"<<c->getPlaceType()<<";"<<c->getPlace()<<"\n";
    }
    return oss.str();
}

string Controller::planetToString(){
    ostringstream oss;
    for (const auto& pair : planetMap) {
        auto p = pair.second;
        oss << "Planet;" << p->getName()<<";"<<p->getDescription()<<"\n";
    }
    return oss.str();
}
string Controller::spaceshipToString(){
    ostringstream oss;
    for (const auto& pair : spaceshipMap) {
        auto p = pair.second;
        oss << "Spaceship;" << p->getName()<< endl;
    }
    return oss.str();
}

string Controller::missionToString(){
    ostringstream oss;
    for (const auto& pair : missionMap) {
        auto m = pair.second;
        oss << "Mission;" << m->getName()<<";"<< m->getDescription()<<"\n";
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
    // Ajout du personnage à l'équipage du vaisseau auquel il est associé
    for(auto& ship : spaceshipMap)
    {
        if(newCharacter->getPlace() == ship.second->getName()){
            ship.second->addCrewMember(characterMap[newCharacter->getName()]);
            break;
        }
    }


    // Ajout du personnage aux habitants de la planete auquel il est associé
    for(auto& pla : planetMap)
    {
        if(newCharacter->getPlace() == pla.second->getName()){
            pla.second->addNewPlanetResident(characterMap[newCharacter->getName()]);
            break;
        }
    }

}
void Controller::addSpaceship(const shared_ptr<Spaceship>& newSpaceship) {
    spaceshipMap[newSpaceship->getName()] = newSpaceship;
}
void Controller::addPlanet(const shared_ptr<Planet>& newPlanet) {
    planetMap[newPlanet->getName()] = newPlanet;
}
void Controller::addMission(const shared_ptr<Mission>& newMission) {
    missionMap[newMission->getName()] = newMission;
}

/*
void Controller::deleteCharacter(const std::shared_ptr<Character>& character) {
    auto it = characterMap.find(character->getName());
    if (it != characterMap.end()) {
        characterMap.erase(it);
        cout << "Personnage supprimé : " << character->getName() << endl;
    } else {
        cout << "Personnage non trouvé : " << character->getName() << endl;
    }
}
*/

void Controller::deleteCharacter(const string& name) {
    auto it = characterMap.find(name);
    if (it != characterMap.end()) {
        string temp = it->first;
        characterMap.erase(temp);
    }
}

void Controller::deleteSpaceship(const string& name) {
    auto it = spaceshipMap.find(name);
    if (it != spaceshipMap.end()) {
        string temp = it->first;
        spaceshipMap.erase(temp);
    }
#ifdef DEBUG
    for (auto& pair : spaceshipMap){
        auto& ship = pair.second;
        cout << "Nom : " << ship->getName() << endl;
    }
#endif
}

void Controller::deletePlanet(const string& name) {
    auto it = planetMap.find(name);
    if (it != planetMap.end()) {
        string temp = it->first;
        planetMap.erase(temp);
    }
#ifdef DEBUG
    for (auto& pair : planetMap){
        auto& pla = pair.second;
        cout << "Nom : " << pla->getName() << endl;
    }
#endif
}

void Controller::deleteMission(const std::string &name) {
    auto it = missionMap.find(name);
    if (it != missionMap.end()) {
        string temp = it->first;
        missionMap.erase(temp);
    }
#ifdef DEBUG
    for (auto& pair : missionMap){
        auto& mi = pair.second;
        cout << "Nom : " << mi->getName() << endl;
    }
#endif
}

Controller::~Controller()
{

}