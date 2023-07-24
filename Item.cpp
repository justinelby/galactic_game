//
// Created by justi on 10/07/2023.
//

#include "Item.h"
using namespace std;
Item::Item(const string name, const string description, const int effect)
        : name(name), description(description), effect(effect){}

string Item::getName() const {
    return name;
}

string Item::getDescription() const {
    return description;
}

int Item::getEffect() const {
    return effect;
}

weak_ptr<Character>& Item::getOwner() {
    return owner;
}

void Item::setOwner(shared_ptr<Character>& owner) {
    this->owner = owner;
}


Item::~Item() {

}
