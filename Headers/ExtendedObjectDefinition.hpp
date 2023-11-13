#ifndef EXTENDEDOBJECTDEFINITION_HPP
#define EXTENDEDOBJECTDEFINITION_HPP

#include "../Headers/ctModel.hpp"
#include <iostream>

class ExtendedObjectDefinition{
    int ext_dim;
    ctModel motionModel;
    double Ps;
    double eta;
    double tao;
    Matrix M;
    double r;
    double PPP_pruningThreshold;
    double MB_pruningThreshold;
    public:
        ExtendedObjectDefinition(int extension_dimension,
                          const ctModel& motionModel,
                          double survivalProbability,
                          double shapeUncertainty,
                          double forgettingFactor,
                          //const Matrix rotationMatrix,
                          double existanceThreshold,
                          double PPP_pruningThreshold,
                          double MB_pruningThreshold)
                          : ext_dim(extension_dimension),
                            motionModel(motionModel),
                            Ps(survivalProbability),
                            eta(shapeUncertainty),
                            tao(forgettingFactor),
                            //M(rotationMatrix),
                            r(existanceThreshold),
                            PPP_pruningThreshold(PPP_pruningThreshold),
                            MB_pruningThreshold(MB_pruningThreshold){

                            }
    Matrix rotationMatrix(const objStateSpace* object);
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