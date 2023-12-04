#include "../Headers/declarations.hpp"

void merge(ObjectsCollection& PPP_objs,
          std::vector<TrackedObj>* newMBs,
          ExtendedObjectDefinition* extObj){
            double d = extObj->getExtensionDimension();
            int nComponents = PPP_objs.PPP.size();
            double w_bar = std::accumulate(PPP_objs.PPP.begin(), PPP_objs.PPP.end(), 0.0, [](double sum, const UntrackedObj& obj) {
                return sum + obj.w_ppp;
            });
            std::cout << w_bar;
          };