#ifndef DBSCAN_HPP
#define DBSCAN_HPP

#include "Matrix.hpp"  
/* 
   DBSCAN is a clustering algorithm for identifying dense regions in spatial data. It categorizes
   points as core, border, or noise, based on the density of their neighborhoods. The algorithm is
   effective for discovering clusters of varying shapes and handling noisy data.
   The input parameters are:
    Z -> A matrix that include the raw sensor data.
    eps -> A parameter that decide the minimum distance allowed to consider a point as part of cluster
    nrPnts -> minimum number of points allowed in a single cluster.
*/
struct DBSCAN {
    // if id(i) = 0 -> Z(i) is clutter.
    std::vector<int> id;

    // C i two dimensional array. The number of rows indicates how many clusters we have
    // The number of columns indicates how many points do we have at each cluster
    // The values in c represent the columns corresponds to the respective cluster
    std::vector<std::vector<int>> c;
};

class dbscan {
public:
    static DBSCAN run(const Matrix& Z, const std::vector<double>& eps, int minPts);
};

#endif 
