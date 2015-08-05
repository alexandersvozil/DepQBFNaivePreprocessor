//
// Created by svozil on 28.07.15.
//

#include "quantgroup.h"

void quantgroup::setType(std::string string) {
    this->type = string;

}

void quantgroup::addVariable(int v) {
    variables.push_back(v);

}

std::string quantgroup::getType() {
    return type;
}


