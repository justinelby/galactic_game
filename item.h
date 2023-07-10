//
// Created by justi on 10/07/2023.
//

#ifndef JEU_PERSONNAGES_ITEM_H
#define JEU_PERSONNAGES_ITEM_H
#include <string>


using namespace std;
class Item {
public:
    Item(const string& name, const string& description, int effect);
    string getName() const;
    string getDescription() const;
    int getEffect() const;

private:
    string name;
    string description;
    int effect;
};



#endif //JEU_PERSONNAGES_ITEM_H
