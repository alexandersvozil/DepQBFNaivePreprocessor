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



