#include "../Headers/obj.hpp"
#include <random>
#include <cmath>

/**
 * @brief Converts an angle from degrees to radians.
 * @param degrees The angle in degrees.
 * @return The angle in radians.
 */
double deg2rad(double degrees) {
   return degrees * M_PI / 180.0;
}


/**
 * @brief Generates a random number within the specified interval [min, max].
 * @param min The minimum value of the interval.
 * @param max The maximum value of the interval.
 * @return A random number within the specified interval.
 */
double genRanNr(double min, double max){
   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_real_distribution<double> distribution(min, max);
   return distribution(gen);
}

/**
 * @brief Initializes a collection of potential objects in the sensor field of view (PPP).
 * @param tot_exp_objs Total expected objects in the field of view.
 * @param objs_rate Object creation rate.
 * @param dof The degree of freedom parameter for the inverse Wishart distribution.
 * @return The collection of potential objects.
 */
ObjectsCollection initPPP(const double tot_exp_objs,
                        const double objs_rate,
                        const double dof) {
   ObjectsCollection collection;

   double pnt = 0;
   double antObj = 3;
   Matrix V(3,3,0.0);
   V(0,0) = 40;
   V(1,1) = 45;
   V(2,2) = 50;

   for (double ran = 2.5; ran <= 50; ran += 7.5) {
      for (double ang = -50; ang <= 50; ang += 100.0 / antObj) {
            UntrackedObj untrackedObj;
            untrackedObj.X.x = ran * cos(deg2rad(ang));
            untrackedObj.X.y = ran * sin(deg2rad(ang));
            untrackedObj.X.v = genRanNr(0, 20); 
            untrackedObj.X.theta = genRanNr(0, 2 * M_PI); 
            untrackedObj.X.w = 0.1;

            if (ang == -50 || ang == 50 || ran == 47.5) {
               untrackedObj.w_ppp = 0.3;
            } else {
               untrackedObj.w_ppp = 0.08;
            }

            //untrackedObj.P = Matrix(5, 5, 2.0) * Matrix::identity(5);
            untrackedObj.P = Matrix(5,5,0.0);
            untrackedObj.P(0,0) = 2;
            untrackedObj.P(1,1) = 5;
            untrackedObj.P(2,2) = 3;
            untrackedObj.P(3,3) = 0.7;
            untrackedObj.P(4,4) = 0.1;
            untrackedObj.v = dof;
            untrackedObj.V = V;
            untrackedObj.alpha = 1;
            untrackedObj.beta = 1;

            collection.PPP.push_back(untrackedObj);
            pnt++;
      }
      antObj += 2;
   }

   return collection;
}


/**
 * @brief Combines two sets of untracked objects (PPP).
 * @param PPP The first set of untracked objects.
 * @param newPPP The second set of untracked objects to be added.
 */
void combinePPPs(std::vector<UntrackedObj>& PPP, std::vector<UntrackedObj> newPPP){
    PPP.insert(PPP.end(), newPPP.begin(), newPPP.end());
}

/**
 * @brief Combines two sets of tracked objects (MB).
 * @param MB The first set of tracked objects.
 * @param newMB The second set of tracked objects to be added.
 */
void combineMBs(std::vector<TrackedObj>& MB, std::vector<TrackedObj> newMB){
   MB.insert(MB.end(), newMB.begin(), newMB.end());
}


