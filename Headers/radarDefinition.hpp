#ifndef RADARDEFINITION_HPP
#define RADARDEFINITION_HPP
#include "Matrix.hpp"
#include "objectsStack.hpp"

class radarDefinition{
    private:
        // x,y and range rate. These are the three measurements wer are receiving by the radar
        int measurements_dimension = 3;

        // The radar pose relative to ego vehicle
        double relative_x = 0.0;
        double relative_y = 0.0;
        double relative_vel = 0.0;

        // Should match the camera if we had it.
        double radar_fov = 72.0;

        // Object detection probability
        double P_d;

        // Gate size when performing gating on the measurements
        double gatingThreshold;
        double T = 1.0/10.0;
        double ci_clutterIntensity;
        double deathForce = 1.0;

    public:
        radarDefinition(double detectionProbability, double gatingThreshold, double ciClutterIntensity):
                          P_d(detectionProbability),gatingThreshold(gatingThreshold),ci_clutterIntensity(ciClutterIntensity){};
        int getmeasurements_dimension();
        Matrix getMeasurementsCovariance();
        double getRadarDetectionProbability();
        double getCIClutterIntensity();
        double getDeathForce();
        Matrix measurementPrediction(const objStateSpace* object);
        Matrix measurementModel(const objStateSpace* object);
};

#endif