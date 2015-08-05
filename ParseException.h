//
// Created by svozil on 28.07.15.
//

#ifndef QBF_CPP_PARSEEXCEPTION_H
#define QBF_CPP_PARSEEXCEPTION_H

#include <string>
class ParseException {

private:
    std::string message;
public:
    ParseException(std::string string);

    std::string getMessage();
};


#endif //QBF_CPP_PARSEEXCEPTION_H

