#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

#include <cmath> 
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <numeric>

// ctModel is inside extendedObjectDefinition
#include "ExtendedObjectDefinition.hpp"

// Matrix.hpp is inside obj.hpp 
#include "obj.hpp"

#include "radarDefinition.hpp"

#include "dbscan.hpp"

void runRadarTest();

void testMatrix_cpp();

void objectCreationTest();

void TestDBSCAN();

void readCSV();

double deg2rad(double degrees);

double genRanNr(double min, double max); 

void combinePPPs(std::vector<UntrackedObj>& PPP, std::vector<UntrackedObj> newPPP);

void combineMBs(std::vector<TrackedObj>& MB, std::vector<TrackedObj> newMB);

ObjectsCollection initPPP(const double tot_exp_objs,
                          const double objs_rate,
                          const double dof); 

void predict(ObjectsCollection& CurrentObjectsCollection,
             ExtendedObjectDefinition extendedObjDef);

void update(ObjectsCollection& collection,
            Matrix* data,
            radarDefinition* radar,
            ExtendedObjectDefinition* extObj);

class measurements{
    public:
        const Matrix* z;
        Matrix inGated;
        Matrix outGated;
        measurements(const Matrix* measurements):z(measurements){};

        measurements(const measurements& other)
        : z(other.z), inGated(other.inGated), outGated(other.outGated) {}

        measurements& operator=(const measurements& other) {
            if (this != &other) {
                z = other.z;
                inGated = other.inGated;
                outGated = other.outGated;
            }
            return *this;
        }
};

void elipsoidalGating(radarDefinition* radar,
                      ObjectsCollection* objs,
                      const char c,
                      measurements& currMeasurements);

void PPP_update(ObjectsCollection& collection,
                measurements* currMeasurements,
                radarDefinition* radar,
                ExtendedObjectDefinition* extObj);

void newBernoulliBirth(ObjectsCollection& PPP_objs,
                       Matrix* clusterMeas, 
                       radarDefinition* radar, 
                       ExtendedObjectDefinition* extObj);
struct GGIW_result{
    TrackedObj newMB;
    double L;
};

GGIW_result ggiwUpdate(UntrackedObj* obj,
                measurements* z,
                radarDefinition* radar,
                ExtendedObjectDefinition* object);


void merge(ObjectsCollection& PPP_objs,
          std::vector<TrackedObj>* newMBs,
          ExtendedObjectDefinition* extObj);

#endif