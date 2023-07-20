// main.cpp

#include <iostream>
#include <string>
#include "Controller.h"

using namespace std;


void displayAllCharacters(Controller &controller) {
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

void displayAllEnemies(Controller &controller) {
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
void displayAllQuest(Controller &controller) {
    cout << "Affichage des missions : " << endl;
    for( auto it : controller.getQuest()){
        cout << it.second->getName() << " ";
        cout << it.second->getDescription() << " ";
    }
}

void displayControllerItems(Controller &controller){
    if(controller.getInventory().empty()){
        cout << "Empty Game Inventory !" << endl;
    }
    else
    {
        cout << "Game inventory : " << endl;
        for( auto &it : controller.getInventory()){
            if(it.second != nullptr)
            {
                cout << it.second->getName() << endl;
                cout << it.second->getDescription() << endl;
                cout << it.second->getEffect() << endl;
                cout << "--------" << endl;
            }
        }

    }
}

void displayCharacterInventory(shared_ptr<Character>& character) {
    cout << character->getName() << "'s inventory : " << endl;
    if (character->getInventory().empty()) {
        cout << "Empty inventory !" << endl;
    } else {
        for (auto &it: character->getInventory()) {
            cout << it->getName() << " ";
            cout << it->getDescription() << " ";
            cout << it->getEffect() << endl;
        }
    }
}

void planetDeleteTest(Controller &controller){
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

void spaceshipDeleteTest(Controller &controller){
    string line;
    bool result;

    do {
    cout << "Nom du vaisseau a supprimer : "<< endl;
    getline(cin, line);
    result = controller.deleteSpaceship(line);
    } while (result == false);
}

void deleteQuestTest(Controller &controller){
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

void debugDisplayWeakPtrNbr(Controller &controller){
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

void displayAllInfo(Controller &controller){
    displayAllCharacters(controller);
    cout << "---------------------" << endl;

    displayAllEnemies(controller);
    cout << "---------------------" << endl;

    displayAllQuest(controller);
    cout << "---------------------" << endl;

    displayControllerItems(controller);
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

//    displayAllCharacters(controller);

    displayControllerItems(controller);


    for(auto it = controller.getInventory().begin(); it != controller.getInventory().end(); it++) {
        if(controller.getCharacter()["Alex Starborn"]->getInventory().size() < 5)
            controller.addToCharacterInventory(controller.getCharacter()["Alex Starborn"], it->second);
    }

    cout << "GAME INVENTORY 1 " << endl;
    for (auto& it : controller.getInventory()) {
        if(it.second == nullptr)
            cout << "null print" << endl;
        else
            cout << it.second->getName() << endl;
    }


    cout << "GAME INVENTORY 2 " << endl;
    for (auto& it : controller.getInventory()) {
        if(it.second == nullptr)
            cout << "null print" << endl;
        else
            cout << it.second->getName() << endl;
    }

//    cout << "-----------------" << endl;
//    displayCharacterInventory(alex);
//    cout << "-----------------" << endl;
//    displayControllerItems(controller);
//    cout << "-----------------" << endl;


//    cout << "MOVE POTION POISON 3" << endl;
//    auto poison = std::move(controller.getInventory()["Potion of Poison III"]);
//    cout << "-----------------" << endl;
//    displayControllerItems(controller);

    cout << "ALEX LOOT POISON III ?" << endl;
    controller.looting(controller.getCharacter()["Alex Starborn"], controller.getInventory()["Potion of Poison III"]);

    cout << "ALEX INVENTORY " << endl;
    for (auto& it : controller.getCharacter()["Alex Starborn"]->getInventory()) {
        if(it == nullptr)
            cout << "null print" << endl;
        else
            cout << it->getName() << endl;
    }

    cout << "------------------" << endl;
    cout << "GAME INVENTORY " << endl;
    for (auto& it : controller.getInventory()) {
        if(it.second == nullptr)
            cout << "null print" << endl;
        else
            cout << it.second->getName() << endl;
    }
    cout << "------------------" << endl;



//    auto p1 = make_pair(1, controller.getCharacter()["Alex Starborn"]->getInventory());
    cout << "PRINT : "<< controller.getInventory()["Potion of Healing II"]->getName() << endl;
    cout << "PRINT : "<< controller.getCharacter()["Alex Starborn"]->getInventory().at(1)->getName() << endl;

    cout << "Alex's hp before : " << controller.getCharacter()["Alex Starborn"]->getHealth() <<endl;
    controller.useItem("Alex Starborn", "Potion of Poison III");
    cout << "Alex's hp after Poison III : " << controller.getCharacter()["Alex Starborn"]->getHealth() <<endl;
    controller.useItem("Alex Starborn", "Potion of Healing I");
    cout << "Alex's hp after Healing I: " << controller.getCharacter()["Alex Starborn"]->getHealth() <<endl;
    controller.useItem("Alex Starborn", "Potion of Max Healing");
    cout << "Alex's hp after Healing I: " << controller.getCharacter()["Alex Starborn"]->getHealth() <<endl;








    /*   string p1 = "Alex Starborn", p2 = "Alien 1";

       auto p1Infos = controller.getCharacter()[p1];
       auto p2Infos = controller.getEnemy()[p2];

       cout << p1Infos->getName() << " " << p1Infos->getStatus()[0] << " " << p1Infos->getStatus()[1] << " " << p1Infos->getStatus()[2] << endl;
       cout << p2Infos->getName() << " " << p2Infos->getStatus()[0] << " " << p2Infos->getStatus()[1] << " " << p2Infos->getStatus()[2] << endl;


       bool combatIsOver = controller.neutralAttack(p1,p2);
       if (combatIsOver){
           cout << p1 << " a execute " << p2 << endl;
       }*/

    //displayAllEnemies(controller);

    //controller.saveGame();


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

