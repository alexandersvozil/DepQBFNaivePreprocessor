//
// Created by svozil on 29.07.15.
//

#ifndef QBF_CPP_PREPROCESSING_H
#define QBF_CPP_PREPROCESSING_H


#include "Formula/formula.h"
#include "ParseException.h"
#include "Formula/clause.h"
#include <iostream>
#include <algorithm>

class preprocessing {
public:

    //the preprocessing with a capped number of resolvents
    formula heuristic_nrResolvents(formula in, int nrResolvents);

    //universal reduction
    clause *universalR(clause *inC, formula f);

    //resolve over the two given clauses and a given variable
    clause *resolve(clause *c1, clause *c2, int toResolve, formula f);
};


#endif //QBF_CPP_PREPROCESSING_H
