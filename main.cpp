// main.cpp

#include <iostream>
#include <string>
#include <memory>
#include "GamePlay.h"
#include "Controller.h"

using namespace std;


//class Testp {
//
//public:
//    Testp(const string& nom) : _nom(nom) {
//        cout << "Personnage " << _nom << " cree." << endl;
//    }
//    string _nom;
//private:
//
//};

//Code contenant l'execution du menu et le jeu
int main() {

    string filepath= "data.txt";
    Controller controller(filepath);
    controller.loadGame();

    cout << controller.getCharacter()[0]->getName() << endl;
    cout << controller.getCharacter()[0]->getHealth() << endl;
    controller.getCharacter()[0]->setHealth(22);
    cout << controller.getCharacter()[0]->getHealth() << endl;

    cout << controller.getSpaceship()[0]->getCrew()[0].lock()->getName() << endl;
    cout << controller.getSpaceship()[0]->getCrew()[0].lock()->getHealth() << endl;
    //cout << controller.getPlanet()[0]->getResident()[0].getName() << endl;

    cout << controller.getCharacter()[2]->getName() << endl;
    cout << controller.getCharacter()[2]->getHealth() << endl;
    controller.getCharacter()[2]->setHealth(55);
    cout << controller.getCharacter()[2]->getHealth() << endl;
    cout << controller.getPlanet()[1]->getResident()[0].lock()->getName() << endl;
    cout << controller.getPlanet()[1]->getResident()[0].lock()->getHealth() << endl;


    //Boucle de jeu
//    bool isRunning = true;
//
//    while (isRunning){
//        // Afficher le menu principal
//        displayMenu();
//
//        //Obtenir l'entrée du joueur
//        int choix;
//        cout << "Entrez votre choix : ";
//        cin >> choix;
//
//        // Exécuter l'action correspondante
//        switch (choix) {
//            case 1: {
//                // Récupération du vecteur de character, vaisseau, planet, mission à partir de l'objet "controller"
//                auto character = controller.getCharacter();
//                auto spaceship = controller.getSpaceship();
//                auto planet = controller.getPlanet();
//                auto mission = controller.getMission();
//
//                // Afficher les personnages
//                if (character.empty()) {
//                    cout << "Il n'y a pas de personnages disponibles pour le moment." << endl;
//                } else {
//                    cout << "\n===== Personnages disponibles =====" << endl;
//                    for (auto Character : character) {
//                        cout << Character->getName() << ", " << Character->getPoste() << ", " << Character->getHealth() << ", " << Character->getAttackPower() << ", " << Character->getPlaceType() << ", " << Character->getPlace() << endl;
//                    }
//                    cout << "\n===== Spaceship disponibles =====" << endl;
//                    for (auto Spaceship : spaceship) {
//                        cout << "- " << Spaceship->getName() << endl;
//                    }
//                    cout << "\n===== Planetes existantes =====" << endl;
//                    for (auto Planet : planet) {
//                        cout << "- " << Planet->getName() << " : " << Planet->getDescription() << endl;
//                    }
//                    cout << "\n===== Missions =====" << endl;
//                    for (auto Mission : mission) {
//                        cout << Mission->getName() << " : " << Mission->getDescription() << endl;
//                    }
//                }
//                break;
//            }
//            case 2: {
//                // Parler aux personnages
//                break;
//            }
//            case 3: {
//                // Voyager vers une autre planète
//                break;
//            }
//            case 0: {
//                // Quitter le jeu
//                isRunning = false;
//                break;
//            }
//            default: {
//                cout << "Choix invalide. Veuillez réessayer." << endl;
//                break;
//            }
//        }
//    }






    return 0;
}

