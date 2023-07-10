//
// Created by Mathieu on 06/07/2023.
//
#include <string>
#include "Enemy.h"

Enemy::Enemy(string name, string desc, int hp, int ap, int dp, string placeType, string place)
: Character(name, desc, hp, ap, dp, placeType, place) {
    //srand(static_cast <unsigned int> (time(NULL)));
    setAttackPower(20);

}


void Enemy::setAttackPower(int apMax) {   // apMax describes the max value you can randomly get
    this->ap = rand() % apMax + 1;      // affects randomly enemy's attack between 1 & apMax
}
