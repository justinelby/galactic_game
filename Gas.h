//
// Created by Mathieu on 28/06/2023.
//

#ifndef JEU_PERSONNAGES_GAS_H
#define JEU_PERSONNAGES_GAS_H

#include <iostream>
#include <string>
using namespace std;
class Gas {
public:
    Gas(string, int, int);

private:
    string gasType;
    int power;
    int quantity;
};


#endif //JEU_PERSONNAGES_GAS_H
