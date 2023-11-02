#ifndef OBJ_HPP
#define OBJ_HPP
#include "objectsStack.hpp"
#include "Matrix.hpp"

class obj{
    objStateSpace X;
    Matrix P(5,5);
    Matrix v;
    Matrix V(3,3);
    Matrix alpha;
    Matrix beta;
    Matrix w_ppp;
    Matrix r_MB;

    obj(){

    }
};

class ObjectsCollection{
    std::vector<obj> PPP;
    std::vector<obj> MB;

    ObjectsCollection(){
        
    }
}


#endif