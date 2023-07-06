//
// Created by justi on 27/06/2023.
//

#include "Quest.h"

Quest::Quest(string nom, string description) : name(nom), description(description), isCompleted(false){}

Quest::Quest(istringstream& iss){
    iss >> name >> description;
}

string Quest::getName() const {
    return name;
}

string Quest::getDescription() const {
    return description;
}

void Quest::setIsCompleted(bool completed) {
    this->isCompleted = completed;
}

bool Quest::getIsCompleted() const {
    return isCompleted;
}

Quest::~Quest() {

}
