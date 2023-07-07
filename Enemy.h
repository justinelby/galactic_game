//
// Created by Mathieu on 06/07/2023.
//

#ifndef GALACTIC_GAME_ENEMY_H
#define GALACTIC_GAME_ENEMY_H
#include "Character.h"

class Enemy : public Character {
public:
    // Constructor
    Enemy(string, string, int, int, int, string, string);

    void setAttackPower(int) override;
};


#endif //GALACTIC_GAME_ENEMY_H
