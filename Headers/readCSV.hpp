#ifndef READCSV_HPP
#define READCSV_HPP

#include <string>
#include <vector>

/*
 This function is to read a csv file that includes x,y and velocity informations. 
 The function is storing the csv data in the vectors x_values, y_values and velocities.
*/

void readCSV1(const std::string& filename,
             std::vector<std::vector<double>>& x_values,
             std::vector<std::vector<double>>& y_values,
             std::vector<std::vector<double>>& velocities);

#endif