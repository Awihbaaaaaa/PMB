#include <iostream>
using namespace std;

int main() {
    int n = 100;      // Number of data points
    double step = 0.1; // Time step in seconds
    double initial_x = 0.0; // Initial position
    double velocity = 1.0; // Constant velocity (1 unit per second)

    for (int i = 0; i < n; i++) {
        double time = i * step; // Elapsed time
        double x = initial_x + velocity * time; // Calculate position
        std::cout << time << " " << x << std::endl;
    }

    return 0;
}