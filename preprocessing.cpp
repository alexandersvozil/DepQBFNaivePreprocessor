//
// Created by svozil on 29.07.15.
//

#include "preprocessing.h"

using namespace std;

void preprocessing::heuristic_nrResolvents(formula *in, int nrResolv) {
    map<int, quantgroup *> &qmap = in->getQuantifierMap();
    map<int, vector<clause *>> &omap = in->getOccurenceMap();
    int resCounter = 0;
    for (auto it = qmap.begin(); it != qmap.end(); ++it) {
//        for (int i = 0; i < qmap.size(); i++) {
        int curVar = it->first;//i + 1;
        //cout << "count: "<< qmap.count(curVar)  << endl;
        /*if(qmap.count(curVar) == 0){
            continue;
        }*/


        quantgroup *curGroup = it->second;//qmap.find(curVar)->second;
        //           cout << "CODE 139heur2" << endl;
//            cout << "CODE 139heur3" << endl;
        if (curGroup->getType() == "a") {
            //cout << "CODE 139heur3" << endl;
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


                    clause* k = resolve(c1, c2, curVar, in);
                    //  cout << "test1" << endl;
                    if (k != c1) {
                        /*  cout << "resolved c: ";
                          for(int var : k->getClauseVariables()){
                              cout << var + " ";
                          }
                          cout << endl; */
                        universalR(k, in);
                        add(in, k);
                        resCounter++;
                    } else {
                        continue;
                    }
                }

            }

        }
    }
    enoughClauses:;

    //return *in;
}

void preprocessing::add(formula *f, clause *c) {
    std::map<int, vector<clause *>> &omap = f->getOccurenceMap();

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

//    cout << "subsumed size: " <<  subsumed.size() << endl;
    //  cout << "deduced clause: " ;
    /* for(int cur : c->getClauseVariables()){
         cout << cur << " ";
     }
     cout << endl; */
    for (clause *k : subsumed) {
        /*for(int var : k->getClauseVariables()){
            cout << var << " ";
        }*/
        //cout << "removed" << k << endl;
        //      cout << k  << " size: "<< k->getClauseVariables().size()<< endl;
        f->removeC(k);
    }

    f->addC(c);
    //cout << endl;
    //   cout << "added clause " << c  << " size: "<< c->getClauseVariables().size()<< endl;
}

void preprocessing::universalR(clause *inC, formula *f) {
    //create the returnclause
    //clause* returnclause =  new clause();
    //returnclause->setHExO(inC->getHExO());
    //the vector of variables in the returnclause
    // std::vector<int> rCVariables;
    //get the variable -> quantgroup map
    std::map<int, quantgroup *> &qmap = f->getQuantifierMap();
    //for each variable in the incoming clause
    //for(int var : inC->getClauseVariables()){
    for (auto iter = inC->getClauseVariables().begin(); iter != inC->getClauseVariables().end();) {
        //get the quantifier groupe of var
        quantgroup *corQ = qmap.find(abs(*iter))->second;
        //cout << corQ->getOrdering() << " " << corQ->getType() << endl;
        /*if it is of type "a" we can eventually delete it from the clause if we dont find any existential variable
        depending on it */
//        cout << var << endl;
        if (corQ->getType() == "a") {
            //cout << inC->getHExO() << " " << corQ->getOrdering()<< endl;
            if (inC->getHExO() > corQ->getOrdering()) {
//                rCVariables.push_back(var);
                ++iter;
                continue;
            } else {
                iter = inC->getClauseVariables().erase(iter);
            }
        } else {
            ++iter;
        }


    }
//    delete(inC);
    //   returnclause->setClauseVariables(rCVariables);
    //  return returnclause;

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
    //fill_n(marked,f->getNrVar(),0);
//    std::vector<int>& v = resultClause->getClauseVariables();
    // cout << "clause1 : ";
    for (int curVar : c1->getClauseVariables()) {
        if (abs(curVar) == abs(toResolve)) continue;
        //  cout << curVar << " ";
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

