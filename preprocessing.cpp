//
// Created by svozil on 29.07.15.
//

#include "preprocessing.h"

using namespace std;

void preprocessing::heuristic_nrResolvents(formula *in, int nrResolv, int maxCSize, bool smode) {

    map<int, quantgroup *> &qmap = in->getQuantifierMap();
    map<int, vector<clause *>> &omap = in->getOccurenceMap();
    int resCounter = 0;
    for (auto it = qmap.begin(); it != qmap.end(); ++it) {
        int curVar = it->first;//i + 1;
        quantgroup *curGroup = it->second;//qmap.find(curVar)->second;
        if (curGroup->getType() == "a") {
            if (omap.count(curVar) == 0 || omap.count(-curVar) == 0) {
                continue;
            }
            vector<clause *> &pos = omap.find(curVar)->second;
            vector<clause *> &neg = omap.find(-curVar)->second;
            for (clause *c1 : pos) {
                if(maxCSize != -1  && (c1->getClauseVariables().size()-1) >maxCSize) continue;

                for (clause *c2 : neg) {
                    if(maxCSize!= -1 && (c1->getClauseVariables().size()+c2->getClauseVariables().size()-2) >maxCSize) continue;

                    if (resCounter >= nrResolv) {
                        goto enoughClauses;
                    }

                    clause* k = resolve(c1, c2, curVar, in);
                    if (k != c1) {
                        universalR(k, in);
                        if(smode) {
                           add(in, k);
                        }else {
                            in->addC(k);
                        }
                        resCounter++;
                    } else {
                        continue;
                    }
                }

            }

        }
    }
    enoughClauses:;

    //maybe check for subsumption in any clause
    /*for(clause* curClause: in->getClauses()){
        subsumptionCheck(in,curClause);
    } */
    //return *in;
}
void preprocessing::subsumptionCheck(formula *f, clause*c){

    std::map<int, vector<clause *>> &omap = f->getOccurenceMap();
    vector <clause *> modified;
    int cSize = c->getClauseVariables().size();
    for(int var : c->getClauseVariables()){
        vector<clause * >& k = omap.find(var)->second;
        for(clause* curClause: k){
            if(curClause->isMarked()) continue;
            if(curClause == c) continue;
            int curClauseSize = curClause->getClauseVariables().size();

            curClause->incRSubcount();
            modified.push_back(curClause);

            //curClause is subsumed
            if(curClause->getSubcount() == cSize){
                curClause->setMarked(true);
            }

            //the clause we tried to add is subsumed by curClause
            if(curClause->getSubcount() == curClauseSize){
                c->setMarked(true);
                goto endloop;
            }
        }
    }
    //f->addC(c);
    endloop:;
    for(clause* unm : modified){
        unm->resetSubcount();
    }

}
void preprocessing::add(formula *f, clause *c) {
    // we could also limit the clause size for subsumption
    /*if(c->getClauseVariables().size() > 7){
        f->addC(c);
        return;
    } */
    std::map<int, vector<clause *>> &omap = f->getOccurenceMap();
    vector <clause *> modified;
    int cSize = c->getClauseVariables().size();
    for(int var : c->getClauseVariables()){
        vector<clause * >& k = omap.find(var)->second;
        for(clause* curClause: k){
            if(curClause->isMarked()) continue;
            if(curClause == c) continue;
            int curClauseSize = curClause->getClauseVariables().size();

            curClause->incRSubcount();
            modified.push_back(curClause);

            //curClause is subsumed
            if(curClause->getSubcount() == cSize){
                curClause->setMarked(true);
            }

            //the clause we tried to add is subsumed by curClause
            if(curClause->getSubcount() == curClauseSize){
               goto endloop;
            }
        }
    }
    f->addC(c);
    endloop:;
    for(clause* unm : modified){
       unm->resetSubcount();
    }


    //TODO: mark the clauses, use information about c_1, c_2. They can never be subsumed by the resolvent
    /*
    vector<clause *> subsumed;
    for (int curSub : c->getClauseVariables()) {
        vector<clause *> tmp = omap.find(curSub)->second;
        if (subsumed.empty()) {
            subsumed = tmp;

        } else {
            for (vector<clause *>::iterator it = subsumed.begin(); it != subsumed.end();) {
                bool found = false;
                for (clause *c2 : tmp) {
                    if (*it == c2) {
                        // cout << "EQUAL: " <<  *it << " " << c2 << endl;
                        found = true;
                    }
                }
                if (found == false) {
                    //cout << "BROKE1" << endl;
                    it = subsumed.erase(it);
                    //cout << "BROKE2" << endl;
                } else {
                    ++it;
                }

                if (subsumed.empty()) {
//                  cout << "BROKE" << endl;
                    goto endloop;
                }
            }
        }
    }
    endloop:;

    for (clause *k : subsumed) {
        f->removeC(k);
    }

    f->addC(c); */
}

void preprocessing::universalR(clause *inC, formula *f) {
    //get the variable -> quantgroup map
    std::map<int, quantgroup *> &qmap = f->getQuantifierMap();
    //for each variable in the incoming clause
    for (auto iter = inC->getClauseVariables().begin(); iter != inC->getClauseVariables().end();) {
        //get the quantifier groupe of var
        quantgroup *corQ = qmap.find(abs(*iter))->second;
        /*if it is of type "a" we can eventually delete it from the clause if we dont find any existential variable
        depending on it */
//        cout << var << endl;
        if (corQ->getType() == "a") {
            if (inC->getHExO() > corQ->getOrdering()) {
                ++iter;
                continue;
            } else {
                iter = inC->getClauseVariables().erase(iter);
            }
        } else {
            ++iter;
        }
    }

}

clause *preprocessing::resolve(clause *c1, clause *c2, int toResolve, formula *f) {
    clause *resultClause = new clause();
    resultClause->setHExO(0);

    int biggerord1 = c1->getHExO();
    int biggerord2 = c2->getHExO();
    if (biggerord1 >= biggerord2) {
        resultClause->setHExO(c1->getHExO());
    } else {
        resultClause->setHExO(c2->getHExO());
    }

    bool tautological = false;
    std::vector<int> marked(f->getNrVar() + 1);

    for (int curVar : c1->getClauseVariables()) {
        if (abs(curVar) == abs(toResolve)) continue;
        if (curVar > 0) {
            marked[curVar] = 1;
        } else {
            marked[(-1) * curVar] = -1;
        }
        resultClause->addVar(curVar);

    }

    for (int curVar : c2->getClauseVariables()) {
        if (abs(curVar) == abs(toResolve)) continue;
        if (curVar > 0) {
            if (marked[curVar] == 1) {
                continue;
            } else if (marked[curVar] == -1) {
                tautological = true;
                break;
            } else if (marked[curVar] == 0) {
                resultClause->addVar(curVar);
            }

        } else {
            if (marked[(-1) * curVar] == 1) {
                tautological = true;
                break;
            } else if (marked[(-1) * curVar] == -1) {
                continue;
            } else {
                resultClause->addVar(curVar);
            }

        }
    }
    if (tautological) {
        delete resultClause;
        return c1;
    }
    return resultClause;
}

