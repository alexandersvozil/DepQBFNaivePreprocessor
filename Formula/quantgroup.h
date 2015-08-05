//
// Created by svozil on 28.07.15.
//

#ifndef QBF_CPP_QUANTGROUP_H
#define QBF_CPP_QUANTGROUP_H

#include <string>
#include <vector>

class quantgroup {
private:
    std::string type;
    std::vector<int> variables;
    int ordering;
public:

    int getOrdering() const {
        return ordering;
    }

    void setOrdering(int ordering) {
        quantgroup::ordering = ordering;
    }

    void setType(std::string string);
    void addVariable(int vN);

    std::string getType();

    const std::vector<int> getVariables() {
        return variables;
    }
};


#endif //QBF_CPP_QUANTGROUP_H
