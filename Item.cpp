//
// Created by justi on 10/07/2023.
//

#include "Item.h"
using namespace std;
Item::Item(const string name, const string description, const int effect, const string owner)
        : name(name), description(description), effect(effect), owner(owner){}

string Item::getName() const {
    return name;
}

string Item::getDescription() const {
    return description;
}

int Item::getEffect() const {
    return effect;
}

string Item::getOwner() const {
    return owner;
}

Item::~Item() {

}
