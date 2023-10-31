#include <iostream>
#include <vector>
#include <cmath>
#include "../Headers/Matrix.hpp"
#include "../Headers/ctModel.hpp"
#include "../Headers/objectsStack.hpp"

double ctModel::getT() const{
    return T;
};

Matrix ctModel::F(obj object){
    Matrix jacobianMatrix(5,5);
    double x = object.x;
    double y = object.y;
    double v = object.v;
    double theta = object.theta;
    double w = object.w;

    jacobianMatrix = {{1  ,   0   ,   T*cos(theta+T*w)  , -T*v*sin(theta+T*w)  ,  -T*sin(theta+T*w)*T*v},
                      {0  ,   1   ,   T*sin(theta+T*w)  ,  T*v*cos(theta+T*w)  ,   T*cos(theta+T*w)*T*v},
                      {0  ,   0   ,        1            ,          0           ,         0             },
                      {0  ,   0   ,        0            ,          1           ,         T             },
                      {0  ,   0   ,        0            ,          0           ,         1             }};
    
    return jacobianMatrix;
}

void ctModel::f(obj& object){
    object.x = object.x + T*object.v*cos(object.theta) + T*object.w;
    object.y = object.y + T*object.v*sin(object.theta) + T*object.w;
    double v = object.v;
    object.theta = object.theta + T*object.w;
    object.theta = object.w; 
    
}

// motion noise covariance
Matrix ctModel::Q(double sigmaV, double sigmaOmega){
    Matrix motionNoiceCovariance(5,5);
    motionNoiceCovariance(2,2) = pow(sigmaV,2);
    motionNoiceCovariance(4,4) = pow(sigmaOmega,2);

    return motionNoiceCovariance;
}