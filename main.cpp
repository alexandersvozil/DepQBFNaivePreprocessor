#include <iostream>
#include <assert.h>
#include "parser.h"
#include "FNFException.h"
#include "ParseException.h"
#include "preprocessing.h"
#include <ctime>
#include <thread>
#include <unistd.h>

extern "C" {
#include "qdpll.h"
}

std::string usage();
bool withpp(formula * f, int maxC, int maxCSize , bool subsumption);
bool withoutpp(formula*);
bool feedSolver(formula*);

void freeFormula(formula*);

using namespace std;


clock_t beginC;


int main(int argc, char *argv[]) {
    try {
        int opt;
        int maxClauses = -1;
        int mCSize = -1;
        bool pMode = false;
        bool sMode = false;
        bool dVariablesMode = false;
        bool LDRMode = false;

        if (argc < 2 || argc > 6) {
            cout << usage();
            return -1;
        }
        while ((opt = getopt(argc, argv, "lvc:pP:s")) != -1) {
            switch (opt) {
                case 's':
                    sMode = true;
                    break;
                case 'l':
                    LDRMode = true;
                    break;
                case 'p':
                    maxClauses = -1;
                    pMode = true;
                    break;
                case 'P':
                    maxClauses = stoi(optarg);
                    pMode = true;
                    break;
                case 'c':
                    mCSize = stoi(optarg);
                    break;
                case 'v':
                    dVariablesMode = true;
                    break;

                default:
                    cout << usage();
                    return -1;
            }
        }
        string path = argv[optind];

        if(LDRMode + dVariablesMode + pMode > 1 ){
            cout << usage();
            return 2;
        }
        parser p;
        double elapsed_secs = 0;
        string modestring;
        bool result;
        formula f_1;
        p.parse(path, &f_1);
        int subsD = 0, resC = 0, varD = 0, rescB = 0;
        if (pMode) {
            //result = withpp(&f_1, maxClauses, mCSize, sMode);

            preprocessing pp;
            if(maxClauses == -1 && sMode) {
                maxClauses = ceil(f_1.getNrClause()*0.05);
            }
            if(maxClauses == -1 && !sMode){ maxClauses = ceil(f_1.getNrClause()*0.05);
                //cout << maxClauses << " csize: " << f_1.getNrClause() << endl;
            }

            beginC = clock();
            pp.heuristic_nrResolvents(&f_1, maxClauses, mCSize, sMode);
            result = feedSolver(&f_1);
            clock_t end1 = clock();
            elapsed_secs = double(end1 - beginC) / CLOCKS_PER_SEC;
            modestring = "preprocessing";

            subsD = pp.dBySubS;
            resC = pp.addedClausesByResolution;
            varD = pp.removedVariablesByUR;
            rescB = pp.rescB;

        }else if(LDRMode){
            preprocessing pp;
            beginC = clock();
                maxClauses = ceil(f_1.getNrClause()*0.05);
               // cout << maxClauses << " csize: " << f_1.getNrClause() << endl;
            /*if(maxClauses = -1 && sMode) {
                maxClauses = f_1.getNrClause()*0.1;
            }*/
            pp.heuristic_LDR_nrResolvents(&f_1,maxClauses,mCSize,sMode);
            result = feedSolver(&f_1);
            clock_t end2 = clock();
            elapsed_secs = double(end2 - beginC) / CLOCKS_PER_SEC;
            modestring = "LDR";

            subsD = pp.dBySubS;
            resC = pp.addedClausesByResolution;
            varD = pp.removedVariablesByUR;
            rescB = pp.rescB;

        }else  {
            result = withoutpp(&f_1);
            clock_t end2 = clock();
            elapsed_secs = double(end2 - beginC) / CLOCKS_PER_SEC;
            modestring = "without";
        }

        cout << "RESULT: " << "elapsed: " << elapsed_secs << " result: " << result << " mode: " << modestring << "." <<
                              endl;
        if(LDRMode || pMode) {
            if(sMode) {
                cout << "Clauses deleted by subsumption: " << subsD << endl
                << "Resolved clauses that are subsumed by the original clauses: " << rescB<< endl;
            }
            cout << "Clauses added by resolution: " << resC << endl
            << "Variables deleted by universal reduction: " << varD << endl;
        }


        freeFormula(&f_1);

    } catch (FNFException f) {
        cout << f.getMessage() << endl;
        return -1;

    } catch (ParseException f) {
        cout << f.getMessage() << endl;
        return -1;
    } catch (invalid_argument){
        cout << "please provide integer arguments for the respective options" << endl;
        cout << usage();
        return -1;
    }

    return 0;
}

void freeFormula(formula* formula1) {
    for(clause* c : formula1->getClauses()){
        delete c;
    }
    for(quantgroup* q: formula1->getQuantgroups()){
        delete q;
    }
}


std::string usage() {
    return "USAGE: qbf_cpp [-c maximum clause size] [-p]  [-P maximum number of added clauses]] [-s] [-l] path_to_qdimacs_file\n -p,-l means that 5% "
            "are the maximum number of added clauses -l uses LDR and p QU-resolution, -P lets you define the number in total, -c lets you restrict the maximum "
            "clause size of the resolved and added clauses \n";
}


//use no heuristic
bool withoutpp(formula* f){
    beginC = clock();
    return feedSolver(f);
}


//use the nrResolvents heuristics
bool withpp(formula *f, int nrResolvents, int maxNrRes, bool b) {
    //TODO: implement the nrResolvents as input parameter
    preprocessing pp;
    beginC = clock();
    if(nrResolvents == -1 && b) nrResolvents = f->getNrClause()*0.10;
    if(nrResolvents == -1 && !b) nrResolvents = f->getNrClause()*0.03;
    pp.heuristic_nrResolvents(f, nrResolvents, maxNrRes, b);
    return feedSolver(f);

}

//feed the formula object to depQBF
bool feedSolver(formula *f){
    QDPLL *depqbf = qdpll_create ();

    /* Use the linear ordering of the quantifier prefix. */
    //const char  *
    string  conf_1 = "--dep-man=simple";
    string conf_2 = "--incremental-use";
    qdpll_configure (depqbf,(char *) conf_1.c_str() );
    /* Enable incremental solving. */
    qdpll_configure (depqbf, (char *) conf_2.c_str() );
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




