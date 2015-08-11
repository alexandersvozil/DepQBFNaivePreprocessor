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


private:
    bool marked;
public:
    clause() : marked(false) {}
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

    bool isMarked() const {
        return marked;
    }

    void setMarked(bool marked) {
        clause::marked = marked;
    }

};


#endif //QBF_CPP_CLAUSE_H
