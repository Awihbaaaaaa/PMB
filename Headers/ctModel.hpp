#ifndef CTMODEL_HPP
#define CTMODEL_HPP
#include "Matrix.hpp"
#include <vector>
#include "../Headers/obj.hpp"

class ctModel{
    private:
        double T;
        int state_dimension = 5;
    
    public: 
        ctModel(double T,double  sigmaV,double  sigmaOmega):T(T){};
        double getT() const;
        Matrix F(objStateSpace object);
        void f(objStateSpace& object);
        Matrix Q(double sigmaV, double sigmaOmega);
};

#endif