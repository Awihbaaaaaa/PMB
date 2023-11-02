#include "../Headers/radarDefinition.hpp"
#include <assert.h>
#include <cmath>

void runRadarTest(){
    // Create a radarDefinition object
    radarDefinition radar(0.8,100,0.6);

    // Test measurementModel
    objStateSpace testObject;
    testObject.x = 10.0;
    testObject.y = 5.0;
    testObject.v = 20.0;
    testObject.theta = 0.0;
    testObject.w = 0.1;

    // Test measurementPrediction
    Matrix Z = radar.measurementPrediction(&testObject);
    Matrix H = radar.measurementModel(&testObject);

    Matrix expectedMeasurementsPrediction(3, 1);
    expectedMeasurementsPrediction = {{11.9999}, {5.02}, {17.957}};

    Matrix expectedMeasurementModel(3, 5);
    expectedMeasurementModel = {{1.0     , 0.0      , 0.099995, -0.00199997, -0.0199997},
                        {0.0     , 1.0      , 0.000999, 0.19999    , 1.9999    },
                        {0.364908, -0.729817, 0.88991 , -9.12271   , -0.912271 }};
    double tolerance = 1e-3;  // Adjust as needed

    for (int i = 0; i < Z.nrRows(); i++) {
        for (int j = 0; j < Z.nrCols(); j++) {
            assert(std::abs(H(i, j) - expectedMeasurementModel(i, j)) < tolerance);
        }
    }

    std::cout << "Expected measurement model test, passed.\n";

}
