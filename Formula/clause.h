//
// Created by svozil on 28.07.15.
//

#ifndef QBF_CPP_CLAUSE_H
#define QBF_CPP_CLAUSE_H
#include <vector>

class clause {
private:
    std::vector<int> clauseVariables;
    int hExO;
public:
    //adds a given variable to a clause
    void addVar(int i);


    int getHExO() {
        return hExO;
    }

    void setHExO(int hExO) {
        clause::hExO = hExO;
    }

    std::vector<int> &getClauseVariables() {
        return clauseVariables;
    }


    void setClauseVariables( std::vector<int> &clauseVariables) {
        clause::clauseVariables = clauseVariables;
    }

};


#endif //QBF_CPP_CLAUSE_H
