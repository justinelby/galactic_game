cmake_minimum_required(VERSION 3.25)
project(jeu_personnages)

set(CMAKE_CXX_STANDARD 17)

add_executable(jeu_personnages main.cpp Character.cpp Character.h Spaceship.cpp Spaceship.h Planet.cpp Planet.h Mission.cpp Mission.h GamePlay.h Controller.cpp Controller.h)
