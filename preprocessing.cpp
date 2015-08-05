//
// Created by svozil on 29.07.15.
//

#include "preprocessing.h"

using namespace std;
formula preprocessing::heuristic_nrResolvents(formula in, int nrResolv){
    map<int,quantgroup*>  &qmap = in.getQuantifierMap();
    map<int,vector<clause*>> &omap= in.getOccurenceMap();
    int resCounter = 0;
        for (int i = 0; i < qmap.size(); i++) {
            int curVar = i + 1;
            quantgroup *curGroup = qmap.find(curVar)->second;
            if (curGroup->getType() == "a") {
                if (omap.count(curVar) == 0 || omap.count(-curVar) == 0) {
                    continue;
                }
                vector<clause *> &pos = omap.find(curVar)->second;
                vector<clause *> &neg = omap.find(-curVar)->second;
                for (clause *c1 : pos) {
                    for (clause *c2 : neg) {
                        if (resCounter >= nrResolv) {
                            goto enoughClauses;
                        }
                        clause *k = resolve(c1, c2, curVar, in);
                        if (k != c1) {
                            if (k->getClauseVariables().size() < 3) {
                                //cout << "ADDED A CLAUSE" << endl;
                                //if we add a clause, the maps are not updated.
                                in.addC(k);
                                resCounter = resCounter + 2;
                            }
                            resCounter++;
                        }else{
                            // cout << "TAUTOLOGICAL CLAUSE RECEIVED" << endl;
                            //resCounter++;
                            continue;
                        }
                    }

                }

            }
        }
    enoughClauses:

    return in;
}

clause* preprocessing::universalR(clause* inC, formula f){
    //create the returnclause
    clause* returnclause =  new clause();
    //the vector of variables in the returnclause
    std::vector<int> rCVariables;
    //get the variable -> quantgroup map
    std::map<int,quantgroup*>& qmap = f.getQuantifierMap();
    //for each variable in the incoming clause
    for(int var : inC->getClauseVariables()){
        //get the quantifier groupe of var
        quantgroup* corQ = qmap.find(abs(var))->second;
        /*if it is of type "a" we can eventually delete it from the clause if we dont find any existential variable
        depending on it */
        if(corQ->getType() == "a"){
            if(inC->getHExO()>corQ->getOrdering()){
                rCVariables.push_back(var);
            }
        }else{
            rCVariables.push_back(var);
        }


    }
    returnclause->setClauseVariables(rCVariables);
    return returnclause;

}

clause* preprocessing::resolve(clause* c1, clause* c2, int toResolve, formula f){
    clause* resultClause = new clause();
    resultClause->setHExO(0);

    bool tautological = false;
    std::vector<int>& v = resultClause->getClauseVariables();
    for(int curVar : c1->getClauseVariables()){
        if(abs(curVar) == abs(toResolve)) continue;
        resultClause->addVar(curVar);

    }

    if(!tautological) {
        for (int curVar : c2->getClauseVariables()) {
            if(abs(curVar) == abs(toResolve)) continue;
            int curVarNeg = -curVar;
            //tautological clause
            if ((std::find(v.begin(), v.end(), curVarNeg) != v.end())) {
                tautological = true;
                break;
            }
            if (!(std::find(v.begin(), v.end(), curVar) != v.end())) {
                resultClause->addVar(curVar);
            }
        }
    }
    if(tautological) {
        return c1;
    }
    return  resultClause;
}
