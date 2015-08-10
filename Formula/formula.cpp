//
// Created by svozil on 28.07.15.
//

#include "formula.h"

void formula::addQG(quantgroup* k) {
    this->quantgroups.push_back(k);
}

void formula::addC(clause *Clause) {
    clauses.push_back(Clause);

}


void formula::removeC(clause *pClause) {
    for(int i=0; i<clauses.size(); i++){
        if(pClause == clauses[i]){

            clauses[i]->setMarked(true);
            //clauses.erase(clauses.begin()+i);

        }
    }

}
