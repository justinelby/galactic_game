//
// Created by Mathieu on 28/06/2023.
//
#include <iostream>
#include <string>
#include "Food.h"


Food::Food(string name, string descr, int fullness) :name(name), descr(descr), fullness(fullness) { }

const string &Food::getDescr() { return descr; }
const string &Food::getName() { return name; }

int Food::getFullness() const { return fullness; }
