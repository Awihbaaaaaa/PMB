#include "../Headers/obj.hpp"
#include <random>
#include <cmath>

/* ObjectsCollection initPPP(const double tot_exp_objs,
                        const double objs_rate,
                        const double dof); */

double deg2rad(double degrees) {
   return degrees * M_PI / 180.0;
}

double genRanNr(double min, double max){
   std::random_device rd;
   std::mt19937 gen(rd());
   std::uniform_real_distribution<double> distribution(min, max);
   return distribution(gen);
}

ObjectsCollection initPPP(const double tot_exp_objs,
                        const double objs_rate,
                        const double dof) {
   ObjectsCollection collection;

   double pnt = 0;
   double antObj = 3;
   Matrix V(3,3);

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

            untrackedObj.P = Matrix(5, 5, 2.0) * Matrix::identity(5);
            untrackedObj.v = dof;
            untrackedObj.V = V;

            collection.PPP.push_back(untrackedObj);
            pnt++;
      }
      antObj += 2;
   }

   return collection;
}


void combineComponents(std::vector<UntrackedObj>& PPP, std::vector<UntrackedObj> newPPP){
    for(UntrackedObj tempObj:newPPP){
        PPP.push_back(tempObj);
    }
}
