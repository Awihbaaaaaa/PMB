#ifndef DBSCAN_HPP
#define DBSCAN_HPP

#include "Matrix.hpp"  

struct DBSCAN {
    std::vector<int> id;

    /* C is a two-dimensional array for cluster information.
    *
    * This array is used to store cluster information where the number of rows indicates the total number of clusters,
    * and the number of columns represents the count of points in each respective cluster. The values in the array correspond
    * to the columns associated with the specific cluster.
    */
    std::vector<std::vector<int>> c;

    friend std::ostream& operator<<(std::ostream &os, const DBSCAN &dbscanResult);
};

class dbscan {
public:
    /**
     * @brief Run the DBSCAN clustering algorithm on the provided spatial data.
     *
     * This static method applies the DBSCAN algorithm to identify clusters in spatial data. It takes a matrix of raw sensor data,
     * a vector specifying the minimum distance allowed (eps) for considering points as part of a cluster, and an integer specifying
     * the minimum number of points (minPts) required to form a cluster.
     *
     * @param Z A Matrix representing the raw sensor data.
     * @param eps A vector of doubles specifying the minimum distance allowed for considering points as part of a cluster.
     * @param minPts An integer representing the minimum number of points required to form a cluster.
     *
     * @return A DBSCAN structure containing the cluster assignments and indices of points in each cluster.
     */
    static DBSCAN run(const Matrix& Z, const std::vector<double>& eps, int minPts);
};

#endif 
