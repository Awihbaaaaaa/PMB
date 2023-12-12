#ifndef CTMODEL_HPP
#define CTMODEL_HPP
#include "Matrix.hpp"
#include <vector>
#include "../Headers/obj.hpp"

/**
 * @brief The `ctModel` class represents a Coordinated Turn Motion Model, providing a robust framework
 * for simulating the dynamic behavior of entities undergoing coordinated turns in a two-dimensional space.
 *
 * To instantiate this class, three essential parameters are required:
 *          
 * @param T The sampling time for the model.
 * @param sigmaV Standard deviation of motion noise added to polar velocity.
 * @param sigmaOmega Standard deviation of motion noise added to the turn rate.
 *
 * The class encapsulates the necessary functionality to compute state transitions, update object states,
 * and generate the process noise covariance matrix for accurate modeling of coordinated turns.
 */
class ctModel {
private:
    double T;               ///< Sampling time.
    double sigmaV;          ///< Standard deviation of motion noise added to polar velocity.
    double sigmaOmega;      ///< Standard deviation of motion noise added to the turn rate.
    int state_dimension = 5;///< Dimension of the state space.

public:
    /**
     * @brief Constructs a Coordinated Turn Motion Model instance with specified parameters.
     * @param T Sampling time.
     * @param sigmaV Standard deviation of motion noise added to polar velocity.
     * @param sigmaOmega Standard deviation of motion noise added to the turn rate.
     */
    ctModel(double T, double sigmaV, double sigmaOmega): T(T), sigmaV(sigmaV), sigmaOmega(sigmaOmega) {};

    /**
     * @brief Gets the sampling time of the model.
     * @return The sampling time.
     */
    double getT() const;

    /**
     * @brief Computes the state transition matrix based on the current object state.
     * @param object The object state space.
     * @return The state transition matrix.
     */
    Matrix F(objStateSpace object);

    /**
     * @brief Updates the object state based on the model's dynamics.
     * @param object The object state space to be updated.
     */
    void f(objStateSpace& object);

    /**
     * @brief Generates the process noise covariance matrix Q for the model.
     * @return The process noise covariance matrix.
     */
    Matrix Q();
};

#endif