#include <iostream>
#include <assert.h>
#include "parser.h"
#include "FNFException.h"
#include "ParseException.h"
#include "preprocessing.h"
#include <ctime>
#include <thread>

extern "C" {
#include "qdpll.h"
}

std::string usage();
bool withpp(formula*);
bool withoutpp(formula*);
bool feedSolver(formula*);

void freeFormula(formula*);

using namespace std;


clock_t beginC;


int main(int argc, char *argv[]) {
    if(argc != 3 && argc != 2) {
        cout << usage();
        return 2;
    }
    string path = argv[1];
    parser p;
    double elapsed_secs=0;
    try {
        string modestring;
        bool result;
        formula f_1 = p.parse(path);
        if(  argc == 3 && std::string(argv[2]) == "-pp") {
            result = withpp(&f_1);
            clock_t end1 = clock();
            elapsed_secs = double(end1 - beginC) / CLOCKS_PER_SEC;
            modestring = "preprocessing";
        }else {
            result = withoutpp(&f_1);
            clock_t end2 = clock();
            elapsed_secs = double(end2 - beginC) / CLOCKS_PER_SEC;
            modestring = "without";
        }
        cout << "RESULT: " << "elapsed: " <<  elapsed_secs << " result: " << result << " mode: " << modestring << "."<< endl;
        freeFormula(&f_1);

    }catch(FNFException f){
        cout << f.getMessage() << endl;

    }catch(ParseException f){
        cout << f.getMessage() << endl;
    }
    return 0;
}

void freeFormula(formula* formula1) {
    for(clause* c : formula1->getClauses()){
      /*  cout << "clause: ";
        for(int k : c->getClauseVariables()){
            cout << k  << " ";
        }
        cout << endl; */
        delete c;
    }
    for(quantgroup* q: formula1->getQuantgroups()){
        delete q;
    }
}


std::string usage() {
    return "USAGE: qbf_cpp path_to_qdimacs_file [use preprocessing -pp]\n";
}


//use no heuristic
bool withoutpp(formula* f){
    beginC = clock();
    return feedSolver(f);
}


//use the nrResolvents heuristics
bool withpp(formula* f){
    //TODO: implement the nrResolvents as input parameter
    preprocessing pp;
    beginC = clock();
    //cout << "size: " <<  f->getClauses().size() << endl;
    pp.heuristic_nrResolvents(f, 50);
    //cout << "size: " <<  f->getClauses().size() << endl;
    return feedSolver(f);

}

//feed the formula object to depQBF
bool feedSolver(formula *f){
    QDPLL *depqbf = qdpll_create ();

    /* Use the linear ordering of the quantifier prefix. */
    char const * conf_1 = "--dep-man=simple";
    char const* conf_2 = "--incremental-use";
    qdpll_configure (depqbf,(char *) conf_1 );
    /* Enable incremental solving. */
    qdpll_configure (depqbf, (char *) conf_2 );
    int nestingLevel = 1;
    for(quantgroup* q : f->getQuantgroups()){
        if(q->getType()=="a"){
            qdpll_new_scope_at_nesting(depqbf, QDPLL_QTYPE_FORALL,nestingLevel);
            for(int curV : q->getVariables()){
                qdpll_add(depqbf,curV);
            }
            qdpll_add(depqbf,0);
        }
        else if(q->getType()=="e"){
            qdpll_new_scope_at_nesting(depqbf, QDPLL_QTYPE_EXISTS,nestingLevel);
            for(int curV : q->getVariables()){
                qdpll_add(depqbf,curV);
            }
            qdpll_add(depqbf,0);

        }else{
            throw ParseException("wrong type detected");
        }
        nestingLevel ++;
    }
    for(clause* cl  : f->getClauses()){
        //this clause is subsumed by a deduced clause
        if(cl->isMarked()) {
//            cout << "dedurced" << cl << endl;
            continue;
        }
        for(int curVar : cl->getClauseVariables()){
            qdpll_add(depqbf,curVar);
        }
        qdpll_add(depqbf, 0);
    }
    int result = qdpll_sat(depqbf);
    qdpll_delete(depqbf);
    //std::cout << result << std::endl;
    if(result == 10){
        return true;
    }else {
        return false;
    }
}




