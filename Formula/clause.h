//
// Created by svozil on 28.07.15.
//

#ifndef QBF_CPP_CLAUSE_H
#define QBF_CPP_CLAUSE_H
#include <vector>

class clause {
private:
    std::vector<int> clauseVariables;
    bool marked;
    int subcount;
    int hExO;
public:

    int getSubcount() const {
        return subcount;
    }

    void resetSubcount(){
        subcount = 0;
    }

    void incRSubcount(){
        subcount ++;
    }
    void setSubcount(int subcount) {
        clause::subcount = subcount;
    }

    clause() : marked(false),subcount(0), hExO(0) {}
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
