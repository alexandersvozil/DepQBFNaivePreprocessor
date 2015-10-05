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

    //the preprocessing with a capped number of resolvents and a maxed Claussize -1 for unlimited
    void heuristic_nrResolvents(formula *in, int nrResolvents, int maxClauseSize, bool smode);

    //adds the clause to the formula and checks for subsumption
    int add(formula *f, clause *c, int i);

    //resolve over the two given clauses and a given variable
    clause *resolve(clause *c1, clause *c2, int toResolve, formula *f);

    //universal reduction
    void universalR(clause *inC, formula *f);

    void subsumptionCheck(formula *f, clause *c);

    void heuristic_deleteVariables(formula *in, int i);

    void heuristic_LDR_nrResolvents(formula *in, int nrResolvents, int maxClauseSize, bool smode);

    clause *resolveLDR(clause *pClause, clause *c2, int var, formula *pFormula);

    int dBySubS = 0;
    int addedClausesByResolution = 0;
    int removedVariablesByUR = 0;
    int rescB = 0;

};


#endif //QBF_CPP_PREPROCESSING_H
