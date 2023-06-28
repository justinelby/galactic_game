// main.cpp

#include <iostream>
#include "GamePlay.h"
#include "Controleur.h"

using namespace std;

//Code contenant l'execution du menu et le jeu
int main() {
    string filepath = "data.txt";
    Controleur controleur(filepath);
    controleur.loadGame();

    //Boucle de jeu
    bool isRunning = true;

    while (isRunning){
        // Afficher le menu principal
        displayMenu();

        //Obtenir l'entrée du joueur
        int choix;
        cout << "Entrez votre choix : ";
        cin >> choix;

        // Exécuter l'action correspondante
        switch (choix) {
            case 1: {
                // Récupération du vecteur de personnage, vaisseau, planete, mission à partir de l'objet "controleur"
                auto personnage = controleur.getPersonnage();
                auto vaisseau = controleur.getVaisseau();
                auto planete = controleur.getPlanete();
                auto mission = controleur.getMission();

                // Afficher les personnages
                if (personnage.empty()) {
                    cout << "Il n'y a pas de personnages disponibles pour le moment." << endl;
                } else {
                    cout << "\n===== Personnages disponibles =====" << endl;
                    for (auto Personnage : personnage) {
                        cout << Personnage.getNom() << ", " << Personnage.getPoste() << ", " << Personnage.getSante() << ", " << Personnage.getPuissance() << ", " << Personnage.getTypeLieu() << ", " << Personnage.getLieu()<< endl;
                    }
                    cout << "\n===== Vaisseau disponibles =====" << endl;
                    for (auto Vaisseau : vaisseau) {
                        cout << "- " << Vaisseau.getNom() << endl;
                    }
                    cout << "\n===== Planetes existantes =====" << endl;
                    for (auto Planete : planete) {
                        cout << "- " << Planete.getNom() << " : " << Planete.getDescription() << endl;
                    }
                    cout << "\n===== Missions =====" << endl;
                    for (auto Mission : mission) {
                        cout << Mission.getNom() << " : " << Mission.getDescription() << endl;
                    }
                }
                break;
            }
            case 2: {
                // Parler aux personnages
                break;
            }
            case 3: {
                // Voyager vers une autre planète
                break;
            }
            case 0: {
                // Quitter le jeu
                isRunning = false;
                break;
            }
            default: {
                cout << "Choix invalide. Veuillez réessayer." << endl;
                break;
            }
        }
    }
    return 0;
}
