#ifndef EXTENDEDOBJECTDEFINITION_HPP
#define EXTENDEDOBJECTDEFINITION_HPP

#include "../Headers/ctModel.hpp"
#include <iostream>

/**
 * @brief The `ExtendedObjectDefinition` class encapsulates parameters defining an extended object's characteristics.
 *
 * This class represents an extended object in a tracking system, incorporating key attributes to model its
 * extent, motion behavior, survival probability, shape uncertainty, and various thresholds for pruning.
 * The parameters include:
 *
 * - `ext_dim`: Object's extent dimension.
 * - `motionModel`: Coordinated Turn motion model describing the object's dynamic behavior.
 * - `Ps`: Object's survival probability.
 * - `eta`: Shape uncertainty for the Gamma distribution.
 * - `tao`: Forgetting factor for the Gamma distribution.
 * - `M`: Object's extent rotation matrix.
 * - `r`: Object's existence probability threshold.
 * - `PPP_pruningThreshold`: Pruning threshold for potentially existent objects.
 * - `MB_pruningThreshold`: Pruning threshold for actual objects.
 *
 * The class provides methods to retrieve specific parameters, such as the extension dimension, survival probability,
 * shape uncertainty, forgetting factor, existence threshold, and pruning thresholds. Additionally, it offers a method
 * to calculate the rotation matrix based on the object's state space.
 */
class ExtendedObjectDefinition{
    int ext_dim; ///< Object's extent dimension
    ctModel motionModel; ///< Coordinated Turn motion model
    double Ps; ///< Object's survival probability
    double eta; ///< shape uncertainty for the Gamma distribution
    double tao; ///< forgetting factor for the gamma disrtibution
    Matrix M; ///< Object's extend rotation matrix 
    double r; ///< Object's existance probability
    double PPP_pruningThreshold; ///< Pruning threshold for potentially existance object
    double MB_pruningThreshold; ///< Prunign threshold for actuall objects
    public:
    /**
    * @brief Constructs an instance of ExtendedObjectDefinition with specified parameters.
    */
        ExtendedObjectDefinition(int extension_dimension,
                          const ctModel& motionModel,
                          double survivalProbability,
                          double shapeUncertainty,
                          double forgettingFactor,
                          double existanceThreshold,
                          double PPP_pruningThreshold,
                          double MB_pruningThreshold)
                          : ext_dim(extension_dimension),
                            motionModel(motionModel),
                            Ps(survivalProbability),
                            eta(shapeUncertainty),
                            tao(forgettingFactor),
                            r(existanceThreshold),
                            PPP_pruningThreshold(PPP_pruningThreshold),
                            MB_pruningThreshold(MB_pruningThreshold){}
    /**
    * @brief Computes the rotation matrix based on the provided object state space.
    * @param object The object's state space.
    * @return The rotation matrix.
    */
    Matrix rotationMatrix(const objStateSpace* object);

    // Getter methods for accessing individual parameters
    int getExtensionDimension() const;
    double getSurvivalProbability() const;
    double getShapeUncertainty() const;
    double getForgettingFactor() const;
    double getExistanceThreshold() const;
    double getPPP_PruningThreshold() const;
    double getMB_PruningThreshold() const;
    const ctModel getMotionModel() const;

};

#endif