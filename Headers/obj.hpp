#ifndef OBJ_HPP
#define OBJ_HPP
#include "Matrix.hpp"

/**
 * @brief Represents the state space of an object.
 */
struct objStateSpace{
    public:
        double x;               ///< X-coordinate
        double y;               ///< Y-coordinate
        double v;               ///< Velocity
        double theta;           ///< Angle
        double w;               ///< Angular velocity

    /**
     * @brief Constructor with values.
     * @param value Array of values [x, y, v, theta, w].
     */
    objStateSpace(const double* value) :  x(value[0]),
                                          y(value[1]),
                                          v(value[2]),
                                          theta(value[3]),
                                          w(value[4]){};
    
    /**
    * @brief Default constructor, initializes all values to 0.0.
    */
    objStateSpace() : x(0.0), y(0.0), v(0.0), theta(0.0), w(0.0) {};

    /**
     * @brief Overloaded ostream operator to print the object state.
     */
    friend std::ostream& operator<<(std::ostream &os, const objStateSpace &obj){
        std::cout << "x = " << obj.x << "\ny = " << obj.y << "\nv = " << obj.v << "\ntheta = " << obj.theta << "\nw = " << obj.w << std::endl;
    }

    /**
     * @brief Overloaded addition operator for adding two state spaces.
     */
    objStateSpace operator+(const objStateSpace& other){
        objStateSpace result;

        result.x = this->x + other.x;
        result.y = this->y + other.y;
        result.v = this->v + other.v;
        result.theta = this->theta + other.theta;
        result.w = this->w + other.w;

        return result;
    }

    /**
     * @brief Overloaded subtraction operator for subtracting two state spaces.
     */
    objStateSpace operator-(const objStateSpace& other){
        objStateSpace result;

        result.x = this->x - other.x;
        result.y = this->y - other.y;
        result.v = this->v - other.v;
        result.theta = this->theta - other.theta;
        result.w = this->w - other.w;

        return result;
    }

};

/**
 * @brief Overloaded addition operator for adding a state space and a matrix.
 */
inline objStateSpace operator+(const objStateSpace& obj, const Matrix& mat) {
    objStateSpace result;
    
    result.x = obj.x + mat(0, 0);
    result.y = obj.y + mat(1, 0);
    result.v = obj.v + mat(2, 0);
    result.theta = obj.theta + mat(3, 0);
    result.w = obj.w + mat(4, 0);

    return result;
}

/**
 * @brief Represents an untracked object with its state space, covariance, and other attributes.
 */
class UntrackedObj{
    public:
        objStateSpace X; ///< Objects's state space
        Matrix P;        ///< State space covariance

        // Inverse Wishart is used to represent the objects' spacial extent 
        // of a Gaussian matrix
        double v;        ///< Scaler that defines the sampling uncertainty (DOF)
        Matrix V;        ///< Scale matrix used to sample the Inverse Wishart

        // Gamma distribution is used to describe how many measurements
        // each object is expected to generate.
        double alpha;   ///< Shape variable for the Gamma distribution
        double beta;    ///< Rate for the Gamma distribution


        double w_ppp;   ///< Weight for potential untracked objects.

    /**
     * @brief Default constructor, initializes members to default values.
     */
    UntrackedObj(): X(),P(5,5), v(), V(3,3), alpha(), beta(), w_ppp() {};

    /**
     * @brief Parameterized constructor to initialize members with provided values.
     * @param state_space Initial state space.
     * @param State_space_covariance Initial state space covariance.
     * @param DOF Degrees of freedom for the Inverse Wishart distribution.
     * @param IW_scaleMatrix Scale matrix for the Inverse Wishart distribution.
     * @param gamma_alpha Shape variable for the Gamma distribution.
     * @param gamma_beta Rate for the Gamma distribution.
     * @param PPP_weights Weight for potential untracked objects.
     */
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

        friend std::ostream& operator<<(std::ostream &os, const UntrackedObj &obj){
        std::cout << obj.X << ", alpha= " << obj.alpha << ", beta= " << obj.beta << ", v = " << obj.v << ".\n V" << obj.V << std::endl;
        }
};

/**
 * @brief Represents a tracked object with its state space, covariance, and other attributes.
 */
class TrackedObj{
    public:
        objStateSpace X; ///< Objects's state space
        Matrix P;        ///< State space covariance

        // Inverse Wishart is used to represent the objects' spacial extent 
        // of a Gaussian matrix
        double v;       ///< Scaler that defines the sampling uncertainty (DOF)
        Matrix V;        ///< Scale matrix used to sample the Inverse Wishart

        // Gamma distribution is used to describe how many measurements
        // each object is expected to generate.
        double alpha;    ///< Shape variable for the Gamma distribution
        double beta;     ///< Rate for the Gamma distribution

        double r_MB;     ///< Survival probability for tracked objects.

    /**
     * @brief Default constructor, initializes members to default values.
     */
    TrackedObj(): X(),P(5,5), v(), V(3,3), alpha(), beta(), r_MB() {};

    /**
     * @brief Parameterized constructor to initialize members with provided values.
     * @param state_space Initial state space.
     * @param State_space_covariance Initial state space covariance.
     * @param DOF Degrees of freedom for the Inverse Wishart distribution.
     * @param IW_scaleMatrix Scale matrix for the Inverse Wishart distribution.
     * @param gamma_alpha Shape variable for the Gamma distribution.
     * @param gamma_beta Rate for the Gamma distribution.
     * @param MB_survival Survival probability for tracked objects.
     */
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

    friend std::ostream& operator<<(std::ostream &os, const TrackedObj &obj){
        std::cout << obj.X << "alpha= " << obj.alpha << ", beta= " << obj.beta << ", v = " << obj.v << ".\nV =\n" << obj.V << std::endl;
    }
};

/**
 * @brief Represents a collection of objects, including untracked and tracked objects.
 */
class ObjectsCollection{
    public:
    std::vector<UntrackedObj> PPP; ///< Vector of potential untracked objects
    std::vector<TrackedObj> MB;    ///< Vector of tracked objects

    /**
     * @brief Default constructor.
     */
    ObjectsCollection(){}
};


#endif