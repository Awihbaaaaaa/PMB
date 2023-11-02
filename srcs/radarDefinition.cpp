#include "../Headers/radarDefinition.hpp"
#include <cmath>

int radarDefinition::getmeasurements_dimension(){
    return this->measurements_dimension;
};

Matrix radarDefinition::getMeasurementsCovariance(){
    Matrix measCovariance(3,3);
    measCovariance = {{0.5, 0  , 0  },
                      {0  , 0.5, 0  },
                      {0  , 0  , 0.5}};
    return measCovariance;
};

double radarDefinition::getRadarDetectionProbability(){
    return this->P_d;
};

double radarDefinition::getCIClutterIntensity(){
    return this->ci_clutterIntensity;
};

double radarDefinition::getDeathForce(){
    return this->deathForce;
};

Matrix radarDefinition::measurementPrediction(const objStateSpace* object){
    Matrix Z(3,1);
    double T = this->T;
    double x = object -> x;
    double y = object -> y;
    double v = object -> v;
    double theta = object -> theta;
    double w = object -> w;
    Z = {{x + T*cos(theta + w*T)*v       },
         {y + T*sin(theta + w*T)*v       },
         {cos(atan2(y,x) + theta + w*T)*v}};
    return Z;
};

Matrix radarDefinition::measurementModel(const objStateSpace* object){
    Matrix H(3,5);
    double x = object -> x;
    double y = object -> y;
    double v = object -> v;
    double theta = object -> theta;
    double w = object -> w;

    H = {{1                                                       , 0                                                           , cos(theta + w/10)/10           , -(w*v*sin(theta + w/10))/10         , -(v*sin(theta + w/10))/10              },
         {0                                                       , 1                                                           , sin(theta + w/10)/10           , (w*v*cos(theta + w/10))/10          , (v*cos(theta + w/10))/10               },
         {v*y*sin(theta + w/10 + atan2(y,x))/(pow(x,2)+(pow(y,2))), -(v*x*sin(theta + w/10 + atan2(y, x)))/(pow(x,2) + pow(y,2)), cos(theta + w/10 + atan2(y, x)), -v*sin(theta + w/10 + atan2(y, x))  , -(v*sin(theta + w/10 + atan2(y, x)))/10}};
    
    return H;
};