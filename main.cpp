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

    string savedFile= "save.txt";
    string loadedFile= "data.txt";
    Controller controller(loadedFile, savedFile);
    controller.loadGame();
    controller.saveGame();

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

    // print crew 1
    cout << "1" << endl;
    auto newChar = make_shared<Character>("Mathieu", "Commandant de bord", 200, 100, "Spaceship","USS Enterprise");
    controller.addCharacter(newChar);
    auto newShip1 = make_shared<Spaceship>("Black Pearl");
    auto newShip2 = make_shared<Spaceship>("Flying Dutch");
    controller.addSpaceship(newShip1);
    controller.addSpaceship(newShip2);

    // affiche Equipage de USS Enterprise
    cout << "entre là " << endl;
    controller.getSpaceship()[0]->showCrew();
    cout << "et là " << endl;
    controller.deleteCharacter(1);  // supprime le Capitaine Anderson

    cout << controller.getSpaceship()[0]->getName() << endl;
    cout << controller.getSpaceship()[1]->getName() << endl;
    cout << controller.getSpaceship()[2]->getName() << endl;
    cout << controller.getSpaceship()[3]->getName() << endl;

    controller.deleteSpaceship(3);

    cout << controller.getSpaceship()[0]->getName() << endl;
    cout << controller.getSpaceship()[1]->getName() << endl;
    cout << controller.getSpaceship()[2]->getName() << endl;


    cout << "3" << endl;
    controller.getSpaceship()[0]->showCrew();

    //controller.getSpaceship()[0]->showCrew();


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

