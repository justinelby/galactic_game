//
// Created by justi on 27/06/2023.
//

#include "Quest.h"

Quest::Quest(string nom, string description, bool isCompleted) : name(nom), description(description), isCompleted(isCompleted){}

string Quest::getName() const {
    return name;
}

string Quest::getDescription() const {
    return description;
}

void Quest::setIsCompleted(const std::string& questName, bool completed) {
    if (name == questName) {
        isCompleted = completed;
    }
}

bool Quest::getIsCompleted() const {
    return isCompleted;
}

Quest::~Quest() {

}
