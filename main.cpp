// main.cpp

#include <iostream>
#include <string>
#include "Controller.h"
#define DEBUG

using namespace std;


/// TESTING FUNCTIONS
void displayAllCharacters(Controller controller) {
    cout << "Affichage des Characters : " << endl;
    for( auto it : controller.getCharacter()){
        cout << it.second->getName() << " ";
        cout << it.second->getHealth() << " ";
        cout << it.second->getAttackPower() << " " ;
        cout << it.second->getArmorPower() << " " ;
        cout << it.second->getPlaceType() << " " ;
        cout << it.second->getPlace() << endl;
    }
}

void displayAllEnemies(Controller controller) {
    cout << "Affichage des Enemies : " << endl;
    for( auto it : controller.getEnemy()){
        cout << it.second->getName() << " ";
        cout << it.second->getHealth() << " ";
        cout << it.second->getAttackPower() << " " ;
        cout << it.second->getArmorPower() << " " ;
        cout << it.second->getPlaceType() << " " ;
        cout << it.second->getPlace() << endl;
    }
}

void displayAllInfo(Controller controller){
    displayAllCharacters(controller);
    cout << "---------------------" << endl;

    displayAllEnemies(controller);
    cout << "---------------------" << endl;

    cout << "Affichage des résidents par Planet : " << endl;
    for( auto it : controller.getPlanet()){
        cout << it.second->getName() << endl;
        for (auto i : it.second->getResident()) {
            cout << i.lock()->getName() << " ";
            cout << i.lock()->getHealth() << " ";
            cout << i.lock()->getAttackPower() << " ";
            cout << i.lock()->getArmorPower() << endl;
        }
        cout << "--------" << endl;
    }
    cout << "---------------------" << endl;
    cout << "Affichage des crewmates par Spaceship : " << endl;
    for( auto it : controller.getSpaceship()){
        cout << it.second->getName() << endl;
        for (auto i : it.second->getCrew()) {
            cout << i.lock()->getName() << " ";
            cout << i.lock()->getHealth() << " ";
            cout << i.lock()->getAttackPower();
            cout << i.lock()->getArmorPower() << endl;
        }
        cout << "--------" << endl;
    }
}

void planetDeleteTest(Controller controller){
    cout << "Liste des planetes : " <<endl;
    for( auto it : controller.getPlanet()){
        cout << it.second->getName() << endl;
    }

    string line;
    bool result;
    do {
        cout << "Nom de la planete a supprimer : "<< endl;
        getline(cin, line);
        result = controller.deletePlanet(line);
    } while (result == false);
}

void spaceshipDeleteTest(Controller controller){
    string line;
    bool result;

    do {
    cout << "Nom du vaisseau a supprimer : "<< endl;
    getline(cin, line);
    result = controller.deleteSpaceship(line);
    } while (result == false);
}

void deleteQuestTest(Controller controller){
    cout << "Liste des missions : " <<endl;
    for( auto it : controller.getQuest()){
        cout << it.second->getName() << endl;
    }

    string line4;
    bool result4;
    do {
        cout << "Nom de la mission a supprimer : "<< endl;
        getline(cin, line4);
        result4 = controller.deleteQuest(line4);
    } while (result4 == false);
}


void debugDisplayWeakPtrNbr(Controller controller){
    cout << "------Nb weak" <<endl;
    int count = 0;
    for( auto it : controller.getPlanet()){
        for (auto character : it.second->getResident()){
            if (!character.expired()){
                count ++;
            }
        }
    }
    cout << count << endl;
}


//Code contenant l'execution du menu et le jeu
int main() {

    string savedFile= "save.txt";
    string loadedFile= "data.txt";
    Controller controller(loadedFile, savedFile);
    controller.loadGame();

    string p1 = "Alex Starborn", p2 = "Alien 1";

    auto p1Infos = controller.getCharacter()[p1];
    auto p2Infos = controller.getEnemy()[p2];

    cout << p1Infos->getName() << " " << p1Infos->getStatus()[0] << " " << p1Infos->getStatus()[1] << " " << p1Infos->getStatus()[2] << endl;
    cout << p2Infos->getName() << " " << p2Infos->getStatus()[0] << " " << p2Infos->getStatus()[1] << " " << p2Infos->getStatus()[2] << endl;


    bool combatIsOver = controller.neutralAttack(p1,p2);
    if (combatIsOver){
        cout << p1 << " a execute " << p2 << endl;
    }

    displayAllEnemies(controller);

    controller.saveGame();


///Boucle de jeu
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
//                auto mission = controller.getQuest();
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

