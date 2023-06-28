//
// Created by Mathieu on 28/06/2023.
//

#include "Mineral.h"
Mineral::Mineral(string name, string descr, int rarity) :name(name), descr(descr), rarity(rarity) { }

const string &Mineral::getDescr() { return descr; }
const string &Mineral::getName() { return name; }

int Mineral::getRarity() const { return rarity; }