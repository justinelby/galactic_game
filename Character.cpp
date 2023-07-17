#include "Character.h"
#define DEBUG

Character::Character(string name, string desc, int hp, int ap, int dp, string placeType, string place)
{
    this->name = name;
    this->descr = desc;
    this->hp = hp;
    this->ap = ap;
    this->dp = dp;
    this->placeType = placeType;
    this->place = place;
}

Character::Character(istringstream& iss) {
    iss >> name >> descr >> hp >> ap >> dp >> placeType >> place;
}

string Character::getName() const { return name; }
string Character::getDescr() const { return descr; }

int Character::getHealth() const { return hp; }
void Character::setHealth(int const hp) { this->hp = hp; }

int Character::getAttackPower() const { return ap; }
void Character::setAttackPower(int ap) { this->ap = ap; }

int Character::getArmorPower() const { return dp; }
void Character::setArmorPower(int dp) { this->dp = dp; }

vector<int> Character::getStatus() { return {hp, ap, dp}; }

string Character::getPlaceType() const { return placeType; }
void Character::setPlaceType(const string &pt) { this->place = pt; }    // add verification if input is valid

string Character::getPlace() const { return place; }
void Character::setPlace(const string &pl) { this->place = pl; }    // add verification if input is valid

map<string, unique_ptr<Item>>& Character::getInventory() { return inventory; }

void Character::addToInventory(unique_ptr<Item>& newItem) {
    if (inventory.size() < 5) {                 // each Character has a 5-item inventory
        inventory[newItem->getName()] = move(newItem);
#ifdef DEBUG
        cout << " added to " << getName() << "'s inventory." << endl;
#endif
    }
    else {
#ifdef DEBUG
        cout << "Item not added to inventory." << endl;
#endif
    }

}

bool Character::isLooting() {
    char response;
    if(inventory.size() >= 5) {
        do {
            cout << "Do you want to replace something in your Inventory (y/N) : " << endl;
            cin >> response;
        } while(response != 'y' or response != 'Y' or response != 'n' or response != 'N');

        if(response == 'y' or response == 'Y')
            return true;
    }
    return false;
}

void Character::looting(unique_ptr<Item>& lootedItem) {
    if(isLooting()) {
        string itemToReplace;
        for (auto &it: inventory) {
            cout << "Name : " << it.second->getName() << endl;
        }
        cout << "Saisir l'Item à remplacer : ";
        cin >> itemToReplace;
        swap(inventory[itemToReplace], lootedItem);
    }
}


Character::~Character()
{
    //dtor
}