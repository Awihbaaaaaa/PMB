#ifndef OBJ_HPP
#define OBJ_HPP
//#include "objectsStack.hpp"
#include "Matrix.hpp"

class objStateSpace{
    public:
        double x;
        double y;
        double v;
        double theta;
        double w;
        objStateSpace * nextObj;

    objStateSpace(const double* value) :  x(value[0]),
                                y(value[1]),
                                v(value[2]),
                                theta(value[3]),
                                w(value[4]),
                                nextObj(nullptr) {};
    objStateSpace() : x(0.0), y(0.0), v(0.0), theta(0.0), w(0.0), nextObj(nullptr) {};

    friend std::ostream& operator<<(std::ostream &os, const objStateSpace &obj);  
};

class UntrackedObj{
    public:
        // Each object representation should include an expectation about 
        // the object's state, the expected amount of measurements that an 
        // object might generate, and a spacial extent representation.
        // Gaussian distribution is used to represent the object's states
        objStateSpace X;// Objects's state space
        Matrix P;  // state space covariance

        // Inverse Wishart is used to represent the objects' spacial extent 
        // of a Gaussian matrix
        double v;       // (DOF) Scaler that defines the sampling uncertainty
        Matrix V;  // Scale matrix used to sample the IW

        // Gamma distribution is used to describe how many measurements
        // each object is expected to generate.
        double alpha;   // Shape variable for Gamma distribution
        double beta;    // Rate


        double w_ppp;   // Weight for potential untracked objects.

    UntrackedObj(): X(),P(5,5), v(), V(3,3), alpha(), beta(), w_ppp() {};
    UntrackedObj(objStateSpace state_space,
        Matrix State_space_covariance,
        double DOF,
        Matrix IW_scaleMatrix,
        double gamma_alpha,
        double gamma_beta,
        double PPP_weights,
        double MB_survival): 
        X(state_space),
        P(State_space_covariance),
        v(DOF),
        V(IW_scaleMatrix),
        alpha(gamma_alpha),
        beta(gamma_beta),
        w_ppp(PPP_weights){}
};

class TrackedObj{
    public:
        // Each object representation should include an expectation about 
        // the object's state, the expected amount of measurements that an 
        // object might generate, and a spacial extent representation.
        // Gaussian distribution is used to represent the object's states
        objStateSpace X;// Objects's state space
        Matrix P;  // state space covariance

        // Inverse Wishart is used to represent the objects' spacial extent 
        // of a Gaussian matrix
        double v;       // (DOF) Scaler that defines the sampling uncertainty
        Matrix V;  // Scale matrix used to sample the IW

        // Gamma distribution is used to describe how many measurements
        // each object is expected to generate.
        double alpha;   // Shape variable for Gamma distribution
        double beta;    // Rate

        double r_MB;    // Survival probability for tracked objects.

    TrackedObj(): X(),P(5,5), v(), V(3,3), alpha(), beta(), r_MB() {};
    TrackedObj(objStateSpace state_space,
        Matrix State_space_covariance,
        double DOF,
        Matrix IW_scaleMatrix,
        double gamma_alpha,
        double gamma_beta,
        double MB_survival): 
        X(state_space),
        P(State_space_covariance),
        v(DOF),
        V(IW_scaleMatrix),
        alpha(gamma_alpha),
        beta(gamma_beta),
        r_MB(MB_survival) {}
};

class ObjectsCollection{
    public:
    std::vector<UntrackedObj> PPP;
    std::vector<TrackedObj> MB;

    ObjectsCollection(){}
};


#endif