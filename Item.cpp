//
// Created by justi on 10/07/2023.
//

#include "Item.h"
using namespace std;
Item::Item(const string& name, const string& description, int effect, int quantity)
        : name(name), description(description), effect(effect), quantity(quantity) {}

string Item::getName() const {
    return name;
}

string Item::getDescription() const {
    return description;
}

int Item::getEffect() const {
    return effect;
}

int Item::getQuantity() const {
    return quantity;
}

void Item::setQuantity(int newQuantity) {
    quantity = newQuantity;
}
