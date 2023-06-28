cmake_minimum_required(VERSION 3.25)
project(jeu_personnages)

set(CMAKE_CXX_STANDARD 17)

add_executable(jeu_personnages main.cpp Personnage.cpp Personnage.h Vaisseau.cpp Vaisseau.h Planete.cpp Planete.h Mission.cpp Mission.h GamePlay.h Controleur.cpp Controleur.h)
