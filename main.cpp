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
    auto characterMap = controller.getCharacter();


    //Testing first character
    if (!controller.getCharacter().empty()) {
        auto firstCharacter = controller.getCharacter().begin()->second;
        cout << "Nom : " << firstCharacter->getName() << endl;
        cout << "Sante : " << firstCharacter->getHealth() << endl;
        firstCharacter->setHealth(22);
        cout << "Sante modifiee : " << firstCharacter->getHealth() << endl;
    } else {
        cout << "Il n'y a pas de personnage disponible." << endl;
    }

    //Testing first spaceship
    cout << "\n------Testing first spaceship crew------" << endl;
    cout << "Nom crew1 : " << controller.getSpaceship()[0]->getCrew()[0].lock()->getName() << endl;
    cout << "Sante crew1 : "  << controller.getSpaceship()[0]->getCrew()[0].lock()->getHealth() << endl;

    //Testing first planet
    cout << "\n------Testing first planet residents------" << endl;
    cout << "Nom resident1 : " << controller.getPlanet()[1]->getResident()[0].lock()->getName() << endl;
    cout << "Sante resident1 : " << controller.getPlanet()[1]->getResident()[0].lock()->getHealth() << endl;

    // Testing character deletion
    cout << "\n------Testing character deletion------" << endl;
    cout << "---Equipage de USS Enterprise crew before character deletion :" << endl;
    controller.getSpaceship()[0]->showCrew();
    controller.deleteCharacter("Capitaine Anderson");  // supprime le Capitaine Anderson dans le vaisseau USS Enterprise
    cout << "---Equipage de USS Enterprise crew after character deletion :" << endl;
    controller.getSpaceship()[0]->showCrew();

    // Testing spaceship deletion
    cout << "\n------Testing spaceship deletion------" << endl;
    cout << "---Character list before spaceship deletion :" << endl;
    for(auto& pair : characterMap){
        auto c = pair.second;
        cout << c->getName() <<endl;
    }
    cout << "---Initial list of spaceships:" << endl;
    for(auto& s : controller.getSpaceship()){
        cout << s->getName() <<endl;
    }
    controller.deleteSpaceship(0);//on supprime USS Enterprise
    cout << "---New list of spaceships:" << endl;
    for(auto& s : controller.getSpaceship()){
        cout << s->getName() <<endl;
    }
    cout << "---Character list after spaceship deletion :" << endl;
    for(auto& pair : characterMap){
        auto c = pair.second;
        cout << c->getName() <<endl;
    }


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

