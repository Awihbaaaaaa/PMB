#include <cmath>
#include "../Headers/ExtendedObjectDefinition.hpp"

Matrix ExtendedObjectDefinition::rotationMatrix(const objStateSpace* object){
    Matrix m(3,3);
    double T = 1.0/10.0;
    m = {{cos(T*object->w), -sin(T*object->w), 0}, 
            {sin(T*object->w), cos(T*object->w) , 0},
            {0              , 0               , 1}};

    return m;
}

const ctModel ExtendedObjectDefinition::getMotionModel() const{
    return this -> motionModel;
}

 int ExtendedObjectDefinition::getExtensionDimension() const{
    return this->ext_dim;
};

double ExtendedObjectDefinition::getSurvivalProbability() const{
    return this->Ps;
}

double ExtendedObjectDefinition::getShapeUncertainty() const{
    return this->eta;
}

double ExtendedObjectDefinition::getForgettingFactor() const{
    return this->tao;
}

double ExtendedObjectDefinition::getExistanceThreshold() const{
    return this->r;
}

double ExtendedObjectDefinition::getPPP_PruningThreshold() const{
    return this->PPP_pruningThreshold;
}

double ExtendedObjectDefinition::getMB_PruningThreshold() const{
    return this->MB_pruningThreshold;
}

