//
// Created by Mathieu on 20/07/2023.
//

#include "Gun.h"

Gun::Gun(string name, string descr, int effect, int ammo)
  : Item(name, descr, effect) {
    this->ammo = maxAmmo;
}

void Gun::setAmmo(int bullets) { ammo = bullets; }

bool Gun::isAmmo() {
    if(ammo > 0)
        return true;
    else
        return false;
}
void Gun::fire() {
    if(isAmmo())
        ammo--;
}

//void reload() { setAmmo(maxAmmo); }