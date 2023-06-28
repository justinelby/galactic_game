//
// Created by justi on 27/06/2023.
//

#include "Mission.h"

Mission::Mission(string nom, string description) : name(nom), description(description), isCompleted(false){}

Mission::Mission(istringstream& iss){
    iss >> name >> description;
}

string Mission::getName() const {
    return name;
}

string Mission::getDescription() const {
    return description;
}

void Mission::setIsCompleted(bool completed) {
    this->isCompleted = completed;
}

bool Mission::getIsCompleted() const {
    return isCompleted;
}

Mission::~Mission() {

}
