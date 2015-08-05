//
// Created by svozil on 28.07.15.
//

#ifndef QBF_CPP_FNFEXCEPTION_H
#define QBF_CPP_FNFEXCEPTION_H


#include <exception>
#include <string>

class FNFException : std::exception{
private:
    std::string message;
public:
    FNFException(std::string Message);
    inline std::string getMessage() { return  message; };
};


#endif //QBF_CPP_FNFEXCEPTION_H
