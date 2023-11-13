#ifndef DECLARATIONS_HPP
#define DECLARATIONS_HPP
// ctModel is inside extendedObjectDefinition
#include "ExtendedObjectDefinition.hpp"

// Matrix.hpp is inside obj.hpp 
#include "obj.hpp"

#include "radarDefinition.hpp"
void runRadarTest();

void testMatrix_cpp();

void objectCreationTest();

void readCSV();

double deg2rad(double degrees);

double genRanNr(double min, double max); 


ObjectsCollection initPPP(const double tot_exp_objs,
                          const double objs_rate,
                          const double dof); 

void predict(ObjectsCollection& CurrentObjectsCollection,
             ExtendedObjectDefinition extendedObjDef);

void update(ObjectsCollection* collection,
            Matrix* data,
            radarDefinition* radar,
            ExtendedObjectDefinition* extObj);


#endif