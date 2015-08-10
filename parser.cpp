//
// Created by svozil on 28.07.15.
//

#include "parser.h"

using namespace std;
formula parser::parse(string qdimacsInputPath) {
    formula resultFormula = formula();
    std::map<int,quantgroup*> qmap = resultFormula.getQuantifierMap();
    std::map<int,std::vector<clause*>> omap = resultFormula.getOccurenceMap();
    ifstream fin;
    fin.open(qdimacsInputPath);
    if(!fin.good()){
        throw FNFException("Couldn't open the given filepath: " + qdimacsInputPath);
    }

    bool firstLine = true;
    int nrVariables=0;
    int nrClauses=0;
    int qGCounter = 0;
    while(!fin.eof()) {

        //parse the first line of qdimacs input
        if (firstLine) {
            string p;
            fin >> p;
            if(p == "c"){
                fin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            if (!(p == "p")) {
                throw ParseException("first letter is not p. Is this QDIMACS format input data?");
            }
            fin >> p;
            if (!(p == "cnf")) {
                throw ParseException("second string is not cnf. Is this QDIMACS format input data?");
            }
            fin >> nrVariables;
            resultFormula.setNrVar(nrVariables);
            fin >> nrClauses;
            firstLine = false;
        }
        string type;
        fin >> type;
        if(type=="") continue;
      //  if(type=="") continue;

        int num = atoi( type.c_str() );

        if (num == 0 && type.c_str()[0] != '0'){
            //quantifier group
            if (type == "e") {
                quantgroup* q = new quantgroup();
                q->setOrdering(qGCounter);
                q->setType("e");
                int curVar;
                fin >> curVar;
                while (curVar != 0) {
                    qmap.insert( std::pair<int,quantgroup*>(curVar,q) );
                    q->addVariable(curVar);
                    fin >> curVar;
                }
                resultFormula.addQG(q);

            } else if (type == "a") {
                quantgroup* q = new quantgroup();
                q->setOrdering(qGCounter);
                q->setType("a");
                int curVar;

                fin >> curVar;
                while (curVar != 0) {
                    qmap.insert( std::pair<int,quantgroup*>(curVar,q) );
                    q->addVariable(curVar);
                    fin >> curVar;
                }

                resultFormula.addQG(q);
            }else{
                throw ParseException("unknown symbol in QDIMACS file: "+type);
            }
            qGCounter++;
        }else{
            /* parse a clause */
            clause* c = new clause();
            c->addVar(num);
            //set highest existential value in clause
            quantgroup* curQ = qmap.find(abs(num))->second;
            if(curQ->getType()=="e"){
                c->setHExO(curQ->getOrdering());
            }

            if(omap.count(num)==0){
                vector<clause*> newVector1 =  std::vector<clause*>();
                omap.insert(std::pair<int,std::vector<clause*>>(num,newVector1));

            }
            std::vector<clause*>& vClauses1 = omap.find(num)->second;
            vClauses1.push_back(c);
           // omap.find(num)->second = vClauses1;
            int curVar;
            fin >> curVar;
            while ( curVar != 0) {

                //set highest existential value in clause
                quantgroup* curQ = qmap.find(abs(curVar))->second;
                if(curQ->getType()=="e" && curQ->getOrdering()>c->getHExO()){
                    c->setHExO(curQ->getOrdering());
                }

                if(!omap.count(curVar)){
                    std::vector<clause*> newVector2 = std::vector<clause*>();
                    omap.insert(std::pair<int,std::vector<clause*>>(curVar,newVector2));

                }
                std::vector<clause*>& vClauses2 = omap.find(curVar)->second;
                vClauses2.push_back(c);
                //omap.find(curVar)->second = vClauses2;
                //std::cout << curVar;
                c->addVar(curVar);
                fin >> curVar;

            }
            resultFormula.addC(c);
           // std::cout << std::endl;
        }

    }
    resultFormula.setOccurenceMap(omap);
    resultFormula.setQuantifierMap(qmap);
    //vector<clause*>* test = resultFormula.getOccurenceMap().find(1)->second;
    /*never had to work with c++ pointers
     * for(clause* c: *test){
        for(int k : c->getClauseVariables()){
            std::cout << "FUCKING TEST " << k << std::endl;
        }

    }

    std::cout << "FUCKING TEST " << test->size() << std::endl; */
    /*for(int i=0; i < qmap.size(); i++){
        quantgroup* curGroup = qmap.find(i+1)->second;
        std::cout<<"variable " << i <<" is contained in the following quantifier group" << std::endl;
        for(int var :curGroup->getVariables()){
            std::cout<< var;
        }
        std::cout<<std::endl;
    }*/
    return  resultFormula;
}

