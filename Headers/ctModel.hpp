#ifndef CTMODEL_HPP
#define CTMODEL_HPP
#include "Matrix.hpp"
#include <vector>
#include "../Headers/obj.hpp"

class ctModel{
    private:
        double T;
        double sigmaV;
        double sigmaOmega;
        int state_dimension = 5;
    
    public: 
        ctModel(double T,double sigmaV,double sigmaOmega):T(T), sigmaV(sigmaV), sigmaOmega(sigmaOmega){};
        double getT() const;
        Matrix F(objStateSpace object);
        void f(objStateSpace& object);
        Matrix Q();
};

#endif