//
// Created by Mathieu on 20/07/2023.
//

#ifndef JEU_PERSONNAGES_GUN_H
#define JEU_PERSONNAGES_GUN_H

#include "Item.h"

class Gun : Item {
public:
    // Constructor
    Gun(string, string, int, int);

    // Getters & Setters
    int getAmmo();
    void setAmmo(int);

    // Methods
    void fire();
    static void reload();

private:
    // Attributes
    int ammo, maxAmmo;

    // Methods
    bool isAmmo();
};


#endif //JEU_PERSONNAGES_GUN_H
