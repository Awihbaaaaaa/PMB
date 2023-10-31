#include "../Headers/readCSV.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void readCSV1(const std::string& filename,
             std::vector<std::vector<double>>& x_values,
             std::vector<std::vector<double>>& y_values,
             std::vector<std::vector<double>>& velocities) {
    // Input file stream is created and opened the csv file
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the CSV file." << std::endl;
        return;
    }

    // Helper variable to loop over all the lines
    std::string line;
    bool firstLine = true;

    // read a line from the input file and store it in line.
    while (std::getline(inputFile, line)) {
        // Check if we are on the first line, skip the loop
        if (firstLine) {
            firstLine = false;
            continue; // Skip the first line
        }

        // 3 vectors to store x,y,vel at each time step
        // The values are extracted first as strings and then casted to double
        std::vector<double> x_values_single, y_values_single, velocities_single;
        std::string x, y, vel;

        // Find the positions of '[' and ']' characters
        // size_t, unsigned int 
        size_t x_start = line.find("[");
        size_t x_end = line.find("]");
        size_t y_start = line.find("[", x_end);
        size_t y_end = line.find("]", y_start);
        size_t vel_start = line.find("[", y_end);
        size_t vel_end = line.find("]", vel_start);

        // The condition is to check if all square brackets are found or not
        // string::npos is the largest possible value of size_t
        if (x_start != std::string::npos && x_end != std::string::npos &&
            y_start != std::string::npos && y_end != std::string::npos &&
            vel_start != std::string::npos && vel_end != std::string::npos) {
            
            // Substring extraction for x, y, vel
            x = line.substr(x_start + 1, x_end - x_start - 1);
            y = line.substr(y_start + 1, y_end - y_start - 1);
            vel = line.substr(vel_start + 1, vel_end - vel_start - 1);

            // Split the x, y, and vel strings by ',' to extract individual values
            std::istringstream x_stream(x), y_stream(y), vel_stream(vel);
            double x_double, y_double, vel_double;

            while (x_stream >> x_double) {
                x_values_single.push_back(x_double);
                if (x_stream.peek() == ',') {
                    x_stream.ignore();
                }
            }

            while (y_stream >> y_double) {
                y_values_single.push_back(y_double);
                if (y_stream.peek() == ',') {
                    y_stream.ignore();
                }
            }

            while (vel_stream >> vel_double) {
                velocities_single.push_back(vel_double);
                if (vel_stream.peek() == ',') {
                    vel_stream.ignore();
                }
            }
        }

        x_values.push_back(x_values_single);
        y_values.push_back(y_values_single);
        velocities.push_back(velocities_single);

        // reset all the temp variables
        x_values_single.clear();
        y_values_single.clear();
        velocities_single.clear();
        x.clear();
        y.clear();
        vel.clear();
    }
}
