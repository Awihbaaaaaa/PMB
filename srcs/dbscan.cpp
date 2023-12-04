#include "../Headers/dbscan.hpp"
#include <cmath>

DBSCAN dbscan::run(const Matrix& Z, const std::vector<double>& eps, int minPts) {
    DBSCAN result;

    int nr_pts = Z.nrCols();

    result.id = std::vector<int>(nr_pts, 0);
    result.c.clear();

    // A clutter trace variable to indicate which point corresponds to which cluster.
    int CNr = 0;

    // A list to track where all the data poits are visited or not
    std::vector<int> visited_ptx(nr_pts, 0);

    // Declare a distance matrix
    Matrix dist(nr_pts, nr_pts, 0.0);

    // Calculate the pairwise distance between the data points
    for (int i = 0; i < nr_pts; ++i) {
        for (int j = 0; j < nr_pts; ++j) {
            double dx = Z(0, i) - Z(0, j);
            double dy = Z(1, i) - Z(1, j);
            dist(i, j) = std::sqrt(dx * dx + dy * dy);
        }
    }

    for (int i = 0; i < nr_pts; ++i) {
        if (!visited_ptx[i]) {
            visited_ptx[i] = 1;

            // Find the neighbours of each and single point
            // and store them in neighbours.
            std::vector<int> neighbours;
            for (int j = 0; j < nr_pts; ++j) {
                if (dist(i,j) <= eps[0]) {
                    neighbours.push_back(j);
                }
            }

            if (neighbours.size() < minPts) {
                // Clutter
                result.id[i] = 0; 
            } else {
                // Found a cluster core
                CNr = CNr + 1;
                result.id[i] = CNr;
                result.c.push_back({i}); ///////////////////////////////

                int j = 0;

                // Check that all points in neighbours belong to the same cluster
                while (j < neighbours.size()) {
                    int pnt_in_cluster = neighbours[j];

                    if (!visited_ptx[pnt_in_cluster]) {
                        visited_ptx[pnt_in_cluster] = 1;
                        std::vector<int> new_neighbour;
                        for (int k = 0; k < nr_pts; ++k) {
                            if (dist(pnt_in_cluster,k) <= eps[0]) {
                                new_neighbour.push_back(k);
                            }
                        }

                        if (new_neighbour.size() >= minPts) {
                            neighbours.insert(neighbours.end(), new_neighbour.begin(), new_neighbour.end());
                        }
                    }

                    if (result.id[pnt_in_cluster] == 0) {
                        result.id[pnt_in_cluster] = CNr;
                        result.c[CNr-1].push_back(pnt_in_cluster);////////////////////
                    }
                    j = j + 1;
                }
            }
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream &os, const DBSCAN &dbscanResult){
    for (int i : dbscanResult.id) {
        std::cout << i << " " ;
    }
};