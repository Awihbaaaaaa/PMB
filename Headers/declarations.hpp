#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP

#include <cmath> 
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

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
        Matrix* z;
        Matrix inGated;
        Matrix outGated;
        measurements(Matrix* measurements):z(measurements){};
};

void elipsoidalGating(radarDefinition* radar,
                      ObjectsCollection* objs,
                      measurements& currMeasurements);

void PPP_update(ObjectsCollection& collection,
                measurements* currMeasurements,
                radarDefinition* radar,
                ExtendedObjectDefinition* extObj);



#endif