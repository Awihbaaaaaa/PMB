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

void testKeyVal();

void testBinaryHeap();

void testAssign2DByCol();

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

struct GGIW_result{
    TrackedObj newMB;
    double L;
};

GGIW_result* newBernoulliBirth(ObjectsCollection& PPP_objs,
                             const Matrix* clusterMeas, 
                             radarDefinition* radar, 
                             ExtendedObjectDefinition* extObj);

template <typename T> 
GGIW_result ggiwUpdate(T* obj,
                measurements* z,
                radarDefinition* radar,
                ExtendedObjectDefinition* object){
    TrackedObj tempObj;

    int m = z->z->nrCols();
    double scalingFactor = 0.25;
    int d = object->getExtensionDimension();
    Matrix R = radar->getMeasurementsCovariance();
    Matrix H = radar->measurementModel(&(obj->X));
   
    // Gamma distribution update
    tempObj.alpha = obj->alpha + m;
    tempObj.beta = obj->beta + 1;

    Matrix x_hat_temp = obj->V*(1/(obj->v-2*d-2));
    Matrix x_hat = (x_hat_temp+x_hat_temp.transpose())/2;
    Matrix R_hat = x_hat*scalingFactor + R;

    // Innovation covariance update
    Matrix S_temp = H*obj->P*H.transpose() + R_hat/m;
    Matrix S = (S_temp+S_temp.transpose())/2;

    // Kalman gain
    Matrix K = obj->P*H.transpose()*S.inv();

    Matrix z_bar = z->z->mean(2);

    // Innovation measurement matrix
    Matrix eps = z_bar - H*obj->X;

    tempObj.X = obj->X + K*eps;

    tempObj.P = obj->P - K*H*obj->P;

    tempObj.v = obj->v + m;

    Matrix s_chol = S.chol();

    Matrix s_chol_transposed = s_chol.transpose();
    Matrix s_chol_transposed_inv = s_chol_transposed.inv();

    Matrix x_hat_chol = x_hat.chol();
    Matrix x_hat_chol_transposed = x_hat_chol.transpose();
    Matrix x_hat_chol_transposed_inv = x_hat_chol_transposed.inv();

    Matrix N = x_hat_chol*s_chol.inv()*(eps*eps.transpose())*s_chol_transposed_inv*x_hat_chol_transposed_inv;

    Matrix z_diff = *(z->z)-z_bar;
    Matrix Z = z_diff*z_diff.transpose();

    Matrix R_hat_chol = R_hat.chol();
    Matrix R_hat_chol_trans = R_hat_chol.transpose();
    Matrix R_hat_chol_trans_inv = R_hat_chol_trans.inv();

    Matrix z_hat = x_hat_chol*R_hat_chol.inv()*Z*R_hat_chol_trans_inv*x_hat_chol_transposed;
    tempObj.V = obj->V+N+z_hat;

    double l1,l2,l3,l4,l5,l6;
    double obj_v_mean = obj->v-d-1;
    double update_v_mean = tempObj.v-d-1;

    l1 = -0.5*d*m*log(M_PI)-0.5*d*log(m);

    double V_det = obj->V.determinant();
    l2 = 0.5*(obj_v_mean)*log(V_det)-0.5*(update_v_mean)*log(tempObj.V.determinant());

    double b_obj = 2*log(M_PI) + lgamma(obj_v_mean/2) + lgamma(obj_v_mean/2-0.5);
    double b_update = 2*log(M_PI) + lgamma(update_v_mean/2) + lgamma(update_v_mean/2-0.5);
    l3 = b_update - b_obj;

    l4 = m*0.5*log(x_hat.determinant())-(m-1)*0.5*log(R_hat.determinant())-0.5*log(S.determinant());

    l5 = lgamma(tempObj.alpha)-lgamma(obj->alpha);

    l6 = obj->alpha*log(obj->beta)-tempObj.alpha*log(tempObj.beta);

    GGIW_result result;
    result.L = l1+l2+l3+l4+l5+l6;
    result.newMB = tempObj;

    return result;
};


TrackedObj merge(std::vector<double>* PPP_weights,
                 std::vector<TrackedObj>* newMBs,
                 ExtendedObjectDefinition* extObj);

double digamma(double x);

double polygamma(int n, double val);

void MB_update(ObjectsCollection& collection,
               measurements* currMeasurements,
               radarDefinition* radar,
               ExtendedObjectDefinition* extObj);

Matrix getClusterMeas(const Matrix* RawMeasInsideGates, std::vector<int>* indices);

void assign2DByCol(std::vector<std::vector<int>>& C, bool maximize,
                   std::vector<int>& col4row, std::vector<int>& row4col, int& gain,
                   std::vector<int>& u, std::vector<int>& v);
                   
#endif