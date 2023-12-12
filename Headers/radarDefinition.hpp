#ifndef RADARDEFINITION_HPP
#define RADARDEFINITION_HPP
#include "Matrix.hpp"
#include "obj.hpp"

/**
 * @brief The `radarDefinition` class represents a radar simulation, capturing the essential parameters and characteristics
 *        necessary for modeling radar measurements and interactions within a tracking system.
 */
class radarDefinition{
    private:
        int measurements_dimension = 3; ///< Measurement dimension (x, y, and range rate).
        double relative_x = 0.0; ///< The radar x pose relative to ego vehicle.
        double relative_y = 0.0; ///< The radar y pose relative to ego vehicle.
        double relative_vel = 0.0; ///< The radar velocity relative to ego vehicle.
        double radar_fov = 72.0; ///< The radar expected field of view. It should match the real radar value.
        double P_d; ///< Object detection probability.
        double gatingThreshold; ///< Gate size when performing gating on the measurements.
        double T = 1.0/10.0; ///< Sampling time.
        double ci_clutterIntensity; ///< Expected clutter intensity.
        double deathForce = 1.0;
        int nr_exp_objs; ///< Expected number of objects that might apear in the field of view.
        int objs_rate; ///< objects rate in the field of view.
        int dof; ///< The degree of freedom represents the number of observations used to estimate the extent covariance matrix.
        int extentCov; ///< Extent convariance.
        double epsCluster; ///< Epsilon parameter for clustering; points within distance epsilon belong to the same cluster.
        int minNrPntsInCluster; ///< minimun nubmer of points allowed in a single cluster.
    public:
        /**
         * @brief Constructs an instance of radarDefinition with specified parameters.
         * @param detectionProbability Object detection probability.
         * @param gatingThreshold Gate size when performing gating on the measurements.
         * @param ciClutterIntensity Expected clutter intensity.
         * @param nrExpObjs Expected number of objects that might appear in the field of view.
         * @param objectsRate Objects rate in the field of view.
         * @param extentDegreesOfFreedom Degrees of freedom for extent.
         * @param extentCovariance Extent covariance.
         * @param epsClusteringThreshold Epsilon parameter for clustering.
         * @param minNrPntsCluster Minimum number of points allowed in a single cluster.
         */
        radarDefinition(double detectionProbability, 
                        double gatingThreshold, 
                        double ciClutterIntensity,
                        int nrExpObjs,
                        int objectsRate,
                        int extentDegreesOfFreedom,
                        int extentCovariance,
                        double epsClusteringThreshold,
                        int minNrPntsCluster):
                          P_d(detectionProbability),
                          gatingThreshold(gatingThreshold),
                          ci_clutterIntensity(ciClutterIntensity),
                          nr_exp_objs(nrExpObjs),
                          objs_rate(objectsRate),
                          dof(extentDegreesOfFreedom),
                          extentCov(extentCovariance),
                          epsCluster(epsClusteringThreshold),
                          minNrPntsInCluster(minNrPntsCluster){};
        // Getter methods for accessing individual parameters.
        int getmeasurements_dimension();
        double getGatingThreshold();
        Matrix getMeasurementsCovariance();
        double getRadarDetectionProbability();
        double getCIClutterIntensity();
        double getDeathForce();
        double getEpsCluster();
        int getMinNrPntsCluster();

        /**
         * @brief Computes the measurement prediction based on a given object's state space.
         * @param object The object's state space.
         * @return The measurement prediction.
         */
        Matrix measurementPrediction(const objStateSpace* object);

        /**
         * @brief Computes the measurement model that defines the relationshipf between  a given object's state space and the radar measurements.
         * @param object The object's state space.
         * @return The measurement model.
         */
        Matrix measurementModel(const objStateSpace* object);
};

#endif