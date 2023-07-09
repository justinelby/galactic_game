// main.cpp

#include <iostream>
#include <string>
#include "Controller.h"
#define DEBUG

using namespace std;


/// AFFICHAGE GENERAL
void displayAllInfo(Controller controller){
    cout << "Affichage des Characters : " << endl;
    for( auto it : controller.getCharacter()){
        cout << it.second->getName() << " ";
        cout << it.second->getHealth() << " ";
        cout << it.second->getAttackPower() << " " ;
        cout << it.second->getArmorPower() << " " ;
        cout << it.second->getPlaceType() << " " ;
        cout << it.second->getPlace() << endl;
    }
    cout << "---------------------" << endl;
    cout << "Affichage des Enemies : " << endl;
    for( auto it : controller.getEnemy()){
        cout << it.second->getName() << " ";
        cout << it.second->getHealth() << " ";
        cout << it.second->getAttackPower() << " " ;
        cout << it.second->getArmorPower() << " " ;
        cout << it.second->getPlaceType() << " " ;
        cout << it.second->getPlace() << endl;
    }
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

//Code contenant l'execution du menu et le jeu
int main() {

    string savedFile= "save.txt";
    string loadedFile= "data.txt";
    Controller controller(loadedFile, savedFile);
    controller.loadGame();

///----------------------------------------------------------------------------
///TEST SUPPRESSION PLANET

/*
    string line;
    bool result;
    do {
        cout << "Nom de la planete a supprimer : "<< endl;
        getline(cin, line);
        result = controller.deletePlanet(line);
    } while (result == false);
    */


///----------------------------------------------------------------------------
///TEST ATTACK FUNCTION

/* Test attack function */
/*    Character& firstCharacter = *(controller.getCharacter().begin()->second);
    Enemy& firstEnemy = *(controller.getEnemy().begin()->second);*/

    /*character attack -> Enemy */
/*    cout << firstEnemy.getName() << " " << firstEnemy.getHealth() << endl;
    controller.characterAttackEnemy(firstCharacter, firstEnemy);
    cout << firstEnemy.getName() << " " << firstEnemy.getHealth() << endl;*/

    /*character attack -> Enemy */
/*
    cout << firstCharacter.getName() << " " << firstCharacter.getHealth() << endl;
    //controller.enemyAttackCharacter(firstEnemy, firstCharacter);
    cout << firstCharacter.getName() << " " << firstCharacter.getHealth() << endl;
*/

///----------------------------------------------------------------------------
///TEST SUPPRESSION MISSION
/*   cout << "Liste des missions : " <<endl;
    for( auto it : controller.getQuest()){
        cout << it.second->getName() << endl;
    }

    string line4;
    bool result4;
    do {
        cout << "Nom de la mission a supprimer : "<< endl;
        getline(cin, line4);
        result4 = controller.deleteQuest(line4);
    } while (result4 == false);*/

    /*cout << "Liste des missions : " <<endl;
    for( auto it : controller.getQuest()){
        cout << it.second->getName() << endl;
    }*/

    controller.saveGame();
/*  cout << "Liste des planetes : " << endl;
    for( auto it : controller.getPlanet()){
        cout << it.second->getName() << endl;
    }

*/




    ///TEST SUPPRESSION VAISSEAU
/*    cout << "Liste des personnages : " <<endl;
    for( auto it : controller.getCharacter()){
           cout << it.second->getName() << endl;
    }
    cout << "Liste des vaisseaux : " <<endl;
    for( auto it : controller.getSpaceship()){
        cout << it.second->getName() << endl;
    }
    string line2;
    bool result2;
    do {
        cout << "Nom du vaisseau a supprimer : "<< endl;
        getline(cin, line2);
        result2 = controller.deleteSpaceship(line2);
    } while (result2 == false);

    cout << "Liste des vaisseaux : " <<endl;
    for( auto it : controller.getSpaceship()){
        cout << it.second->getName() << endl;
    }

    cout << "Liste des personnages : " <<endl;
    for( auto it : controller.getCharacter()){
        cout << it.second->getName() << endl;
    }

    cout << "------Spaceships" <<endl;
    for( auto it : controller.getSpaceship()){
        for (auto character : it.second->getCrew()){
            cout << character.lock()->getName() << endl;
        }
    }*/

/*

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
*/




//
//
//    controller.deleteCharacter("Capitaine Anderson");
//
//    cout << "---Character list after character deletion :" << endl;
//    for (auto& pair : controller.getCharacter()){
//        auto& character = pair.second;
//        cout << "Nom : " << character->getName() << endl;
//    }
//    cout << "\n\n Liste des vaisseaux" <<endl;
//    for (auto& pair : controller.getSpaceship()){
//        auto& ship = pair.second;
//        ship->showCrew();
//    }
//
//










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

