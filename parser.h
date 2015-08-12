//
// Created by svozil on 28.07.15.
//

#ifndef QBF_CPP_PARSER_H
#define QBF_CPP_PARSER_H



#include<string>
#include "Formula/formula.h"
#include "FNFException.h"
#include "ParseException.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

class parser {

public:
    //parse the qDimacsFile
    void parse(std::string qDimacsInput, formula *pFormula);
};


#endif //QBF_CPP_PARSER_H
