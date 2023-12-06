#include "../Headers/declarations.hpp"


TrackedObj merge(std::vector<UntrackedObj>* PPP_objs,
          std::vector<TrackedObj>* newMBs,
          ExtendedObjectDefinition* extObj){
            TrackedObj mergedComponents;
            double d = extObj->getExtensionDimension();
            int nComponents = PPP_objs->size();

            double w_bar = std::accumulate(PPP_objs->begin(), PPP_objs->end(), 0.0, [](double sum, const UntrackedObj& obj) {
                return sum + obj.w_ppp;
            });
            double norm_w = 1/w_bar;
            
            //std::cout << w_bar;
            double alpha_min = 1;
            double v_min = 200;
            std::cout << PPP_objs->size() << "\n";
            std::cout << newMBs->size();
            
            // Initialize the uilding components for merging the Gamma distribution
            double c1=0, c2=0;

            // Initialize the building components for the Inverse Wishart Distribution
            double C1=0, C2=0, C3=0;

            Matrix tmp_P(5,5,0.0), weighted_tmp_P(5,5,0.0);
            
            Matrix V2(d,d,0.0);
            

            for(int i = 0; i<newMBs->size(); i++){
              std::cout << newMBs->at(i).X;
              mergedComponents.X.x += norm_w*newMBs->at(i).X.x*PPP_objs->at(i).w_ppp;
              mergedComponents.X.y += norm_w*newMBs->at(i).X.y*PPP_objs->at(i).w_ppp;
              mergedComponents.X.v += norm_w*newMBs->at(i).X.v*PPP_objs->at(i).w_ppp;
              mergedComponents.X.theta += norm_w*newMBs->at(i).X.theta*PPP_objs->at(i).w_ppp;
              mergedComponents.X.w += norm_w*newMBs->at(i).X.w*PPP_objs->at(i).w_ppp;
              std::cout << mergedComponents.X << std::endl;

              Matrix tmpObjStateSpaceDiff(PPP_objs->at(i).X - mergedComponents.X);
              std::cout << tmpObjStateSpaceDiff;
              
              tmp_P = PPP_objs->at(i).P + tmpObjStateSpaceDiff*tmpObjStateSpaceDiff.transpose();
              weighted_tmp_P = PPP_objs->at(i).w_ppp*tmp_P;
              std::cout << PPP_objs->at(i).P;
              mergedComponents.P = mergedComponents.P + weighted_tmp_P;
              std::cout << mergedComponents.P;
              //tmp_P = PPP_objs->at(i).w_ppp*newMBs->at(i).P + newMBs->at(i).X*
            }
            return mergedComponents;
          };