//
// Created by Mathieu on 20/07/2023.
//

#include "Weapon.h"

Weapon::Weapon(string name, string descr, int power)
    : name(name), descr(descr), power(power) {}

string Weapon::getName() const {
    return name;
}

string Weapon::getDescription() const {
    return descr;
}