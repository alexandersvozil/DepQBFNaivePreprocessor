//
// Created by svozil on 28.07.15.
//

#ifndef QBF_CPP_FORMULA_H
#define QBF_CPP_FORMULA_H

#include <map>
#include "quantgroup.h"
#include "clause.h"

class formula {
private:
    //a list of all quantifier groups
    std::vector <quantgroup*> quantgroups;
    //a list of all clauses
    std::vector <clause*> clauses;
    /*a map from integer -> quantifier group
     * this allows us to quickly how the variable is quantified
     */
    std::map<int,quantgroup*> quantifierMap;
    /*
     * a map from integer ->List<clause>
     * this allows us to quickly check which clauses a variable is in
     */
    std::map<int, std::vector<clause*>> occurenceMap;

private:
    int nrVar;
public:

    int getNrVar() const {
        return nrVar;
    }

    void setNrVar(int nrVar) {
        formula::nrVar = nrVar;
    }

    std::vector<quantgroup *> &getQuantgroups()  {
        return quantgroups;
    }


    std::vector<clause *> &getClauses() {
        return clauses;
    }

    void setClauses(std::vector<clause *> &clauses) {
        formula::clauses = clauses;
    }

    std::map<int, quantgroup *> &getQuantifierMap()  {
        return quantifierMap;
    }

    void setQuantifierMap(std::map<int, quantgroup *> &quantifierMap) {
        formula::quantifierMap = quantifierMap;
    }


    std::map<int, std::vector<clause *>> &getOccurenceMap() {
        return occurenceMap;
    }

    void setOccurenceMap(std::map<int, std::vector<clause *>> &occurenceMap) {
        formula::occurenceMap = occurenceMap;
    }

    void addC(clause *Clause);

    void addQG(quantgroup *k);

    void removeC(clause *pClause);
};


#endif //QBF_CPP_FORMULA_H
