//
// Created by justi on 10/07/2023.
//

#ifndef JEU_PERSONNAGES_ITEM_H
#define JEU_PERSONNAGES_ITEM_H
#include <string>
#include <memory>


using namespace std;

class Character;

class Item {
public:
    Item(const string name, const string description, const int effect);
    string getName() const;
    string getDescription() const;
    int getEffect() const;
    weak_ptr<Character>& getOwner();
    void setOwner(shared_ptr<Character>&);

    virtual ~Item();


private:
    string name;
    string description;
    int effect;
    weak_ptr<Character> owner;
};



#endif //JEU_PERSONNAGES_ITEM_H
