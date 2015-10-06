//
// Created by svozil on 29.07.15.
//

#include "preprocessing.h"

using namespace std;






void preprocessing::heuristic_LDR_nrResolvents(formula *in, int nrResolv, int maxCSize, bool smode) {

    map<int, quantgroup *> &qmap = in->getQuantifierMap();
    map<int, vector<clause *>> &omap = in->getOccurenceMap();
    int resCounter = 0;
    for (auto it = qmap.begin(); it != qmap.end(); ++it) {
        int curVar = it->first;
        quantgroup *curGroup = it->second;//qmap.find(curVar)->second;
        //LDR only considers existential variables
        if (curGroup->getType() == "e") {
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
                    universalR(c1, in);

                //    cout << c1 << " bla:" << c2<< endl;
                    universalR(c2, in);
                    clause* k = resolveLDR(c1, c2, curVar, in);
                    if (k != c1) {
                        if(smode) {
                           resCounter = add(in, k, resCounter);
                        }else {
                            in->addC(k);
                            resCounter++;
                        }
                    } else {
                        continue;
                    }
                }

            }

        }
    }
    enoughClauses:;

    addedClausesByResolution = resCounter;
}

void preprocessing::heuristic_nrResolvents(formula *in, int nrResolv, int maxCSize, bool smode) {

    map<int, quantgroup *> &qmap = in->getQuantifierMap();
    map<int, vector<clause *>> &omap = in->getOccurenceMap();
    int resCounter = 0;
    for (auto it = qmap.begin(); it != qmap.end(); ++it) {
        int curVar = it->first;
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

                    universalR(c1, in);
                    //check if we deleted our curvar
                    bool curvarcheck1 = false;
                    for(int var : c1->getClauseVariables() ){
                        if(abs(var) == curVar){
                            curvarcheck1 = true;
                            break;
                        }
                    }
                    if(!curvarcheck1){
                    //    cout << "we deleted curvar";
                    continue;
                    }

                    universalR(c2, in);

                    bool curvarcheck2 = false;
                    for(int var : c2->getClauseVariables() ){
                        if(abs(var) == curVar){
                            curvarcheck2 = true;
                            break;
                        }
                    }
                    if(!curvarcheck2){
                        //cout << "we deleted curvar";
                    continue;
                    }
                    clause* k = resolve(c1, c2, curVar, in);
                    if (k != c1) {
                        if(smode) {
                            resCounter = add(in, k, resCounter);
                        }else {
                            in->addC(k);
                            resCounter++;
                        }
                    } else {
                        continue;
                    }
                }

            }

        }
    }
    enoughClauses:;

    addedClausesByResolution = resCounter;
    //maybe check for subsumption in any clause
    /*for(clause* curClause: in->getClauses()){
        subsumpti
    } */
    //return *in;
}
int preprocessing::add(formula *f, clause *c, int i) {
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
                dBySubS++;
                curClause->setMarked(true);
            }

            //the clause we tried to add is subsumed by curClause
            if(curClause->getSubcount() == curClauseSize){
                rescB ++;
               goto endloop;
            }
        }
    }
    i++;
    f->addC(c);
    endloop:;
    for(clause* unm : modified){
       unm->resetSubcount();
    }
    return i;
}

void preprocessing::universalR(clause *inC, formula *f) {
    //get the variable -> quantgroup map
    std::map<int, quantgroup *> &qmap = f->getQuantifierMap();
    //for each variable in the incoming clause
    for (auto iter = inC->getClauseVariables().begin(); iter != inC->getClauseVariables().end();) {
        //get the quantifier groupe of var
       // cout << *iter << endl ;
        quantgroup *corQ = qmap.find(abs(*iter))->second;
        /*if it is of type "a" we can eventually delete it from the clause if we dont find any existential variable
        depending on it */
        if (corQ->getType() == "a") {
            //get Ordering produces the highest existential variable of the clause
           // cout << inC->getHExO() << " " << corQ->getOrdering() << endl;
            if (inC->getHExO() > corQ->getOrdering()) {
                //cout << "TEST";
                ++iter;
                continue;
            } else {
                removedVariablesByUR++;
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

clause *preprocessing::resolveLDR(clause *c1, clause *c2, int toResolve, formula *f) {
    clause *resultClause = new clause();
    resultClause->setHExO(0);

    //get smallest existential variable, because we care about existential variables,
    // we also need to check with the variable to resolve
    int biggerord1 = c1->getHExO();
    int biggerord2 = c2->getHExO();
    bool newEx0  = false;
    if(toResolve != biggerord1 && toResolve != biggerord2) {
        if (biggerord1 >= biggerord2) {
            resultClause->setHExO(c1->getHExO());
        } else {
            resultClause->setHExO(c2->getHExO());
        }
    }else{
        newEx0 = true;
        //toResolve is the highest existential value
    }

    bool tautological = false;

    int curnewEx0 = 0;


    map<int,quantgroup*>& quantmap  = f->getQuantifierMap();
    std::vector<int> marked(f->getNrVar() + 1);

    for (int curVar : c1->getClauseVariables()) {
        if (abs(curVar) == abs(toResolve)) continue;
        if (curVar > 0) {
            marked[curVar] = 1;
        } else {
            marked[(-1) * curVar] = -1;
        }

        resultClause->addVar(curVar);
        if(newEx0){
            //replace biggest current quantifier if necessary
           quantgroup* qgroup = quantmap.find(curVar)->second;
            if(qgroup->getType() == "e" && qgroup->getOrdering() > curnewEx0){
                curnewEx0 = qgroup->getOrdering();
            }

        }

    }

    quantgroup* qPivot = quantmap.find(toResolve)->second;

    int pivotordering = qPivot->getOrdering();

    for (int curVar : c2->getClauseVariables()) {
        if (abs(curVar) == abs(toResolve)) continue;
        if (curVar > 0) {
            if (marked[curVar] == 1) {
                continue;
            } else if (marked[curVar] == -1) {
                //we can now losen this constraint
               quantgroup* q =  quantmap.find(abs(curVar))->second;
                if(q->getType() == "e"){
                    tautological = true;
                    break;
                }else{
                   //we have an all quantified x' and need to check if it is smaller than the pivot
                    if(q->getOrdering() > pivotordering){
                        resultClause->addVar(curVar);
                    }else{
                        tautological = true;
                        break;
                    }
                }
            } else if (marked[curVar] == 0) {
                resultClause->addVar(curVar);
            }

        } else {
            if (marked[(-1) * curVar] == 1) {
                //we can now losen this constraint
                quantgroup* q =  quantmap.find(abs(curVar))->second;
                if(q->getType() == "e"){
                    tautological = true;
                    break;
                }else{
                    //we have an all quantified x' and need to check if it is smaller than the pivot
                    if(q->getOrdering() > pivotordering){
                        resultClause->addVar(curVar);
                    }else{
                        tautological = true;
                        break;
                    }
                }
            } else if (marked[(-1) * curVar] == -1) {
                continue;
            } else {
                resultClause->addVar(curVar);
            }

        }
        if(newEx0){
            //replace biggest current quantifier if necessary
            quantgroup* qgroup = quantmap.find(curVar)->second;
            if(qgroup->getType() == "e" && qgroup->getOrdering() > curnewEx0){
                curnewEx0 = qgroup->getOrdering();
            }

        }
    }

    if (tautological) {
        delete resultClause;
        return c1;
    }
    //if the ordering needs to be replaced, to it
    if(newEx0) {
        resultClause->setHExO(curnewEx0);
    }
    return resultClause;
}


// does not work because it produces unsound results
/*void preprocessing::heuristic_deleteVariables(formula *in, int deleteCounter) {


    map<int, quantgroup *> &qmap = in->getQuantifierMap();
    map<int, vector<clause *>> &omap = in->getOccurenceMap();

    int curDCounter = 0;
    for (auto it = qmap.begin(); it != qmap.end(); ++it) {
        vector<clause *> tomark;
        vector<clause *> toAdd;
        //if(curDCounter > deleteCounter){
         //   goto end;
        //}
        int curVar = it->first;
        quantgroup *curGroup = it->second;//qmap.find(curVar)->second;
       // if (curGroup->getType() == "e") continue;

        if (omap.count(curVar) == 0 || omap.count(-curVar) == 0) {
            continue;
        }

        vector<clause *> &pos = omap.find(curVar)->second;
        vector<clause *> &neg = omap.find(-curVar)->second;


        for (clause *c1 : pos) {
            if(c1->isMarked()) continue;
            for (clause *c2 : neg) {
                if(c2->isMarked()) continue;

                clause *k = resolve(c1, c2, curVar, in);

                //k is not tautological
                if (k != c1) {
                    universalR(k, in);
                    //add(in, k);
                    in->addC(k);
                    toAdd.push_back(k);
                    //add the result to the formula
                    tomark.push_back(c1);
                    tomark.push_back(c2);
                }
            }
        }
        cout << "CURVAR: " << curVar << endl;
        cout << "toAdd size:  " << toAdd.size() << endl;
        for(clause* k : toAdd) {
            for (int var : k->getClauseVariables()) {
                cout << var << " ";
                vector<clause *> &clauses = omap.find(var)->second;
                clauses.push_back(k);
            }
            cout << endl;
        }

        cout << "tomark size: "  << tomark.size() << endl;
        for(clause* c : tomark){
            for (int var : c->getClauseVariables()) {
                cout << var << " ";
            }
            cout << endl;
            c->setMarked(true);
        }

        curDCounter++;
    }end:;
} */
