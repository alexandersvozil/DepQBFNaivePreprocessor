//
// Created by svozil on 28.07.15.
//

#include "ParseException.h"

std::string ParseException::getMessage() {
    return  this->message;

}

ParseException::ParseException(std::string string) {
this->message = string;
}
