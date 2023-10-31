#ifndef CTMODEL_HPP
#define CTMODEL_HPP
#include "Matrix.hpp"
#include <vector>
#include "../Headers/objectsStack.hpp"

class ctModel{
    private:
        double T;
        int state_dimension = 5;
    
    public: 
        ctModel(double T,double  sigmaV,double  sigmaOmega):T(T){};
        double getT() const;
        Matrix F(obj object);
        void f(obj& object);
        Matrix Q(double sigmaV, double sigmaOmega);
};

#endif