#include "../Headers/declarations.hpp"

/**
 * @brief Merge multiple untracked objects into a potentially tracked object using a weighted combination.
 *
 * The function merges a collection of untracked objects, each represented by its state estimate, into a single tracked object. 
 * The merging process involves computing weighted sums of various components, including position, covariance matrices, and parameters
 * from the Gamma and Inverse Wishart distributions. The resulting tracked object represents a consolidated estimate that incorporates 
 * information from the individual untracked objects. 
 *
 * @param PPP_weights A vector of untracked objects, each providing a partial state estimate.
 * @param newMBs A vector of tracked objects representing the possible new state estimates of the untracked objects.
 * @param extObj Pointer to the extended object definition, specifying additional characteristics of the object.
 *
 * @return A tracked object resulting from the merging process, containing a consolidated state estimate and updated parameters.
 */
TrackedObj merge(std::vector<double>* PPP_weights, std::vector<TrackedObj>* newMBs, ExtendedObjectDefinition* extObj){
  TrackedObj mergedComponents;
  double d = extObj->getExtensionDimension();
  int nComponents = PPP_weights->size();  
  
  double w_bar = std::accumulate(PPP_weights->begin(), PPP_weights->end(), 0.0);
  double norm_w = 1/w_bar;
  
  double alpha_min = 1;
  double v_min = 200;
  
  // Initialize the building components for merging the Gamma distribution
  double c=0, c1=0, c2=0;

  // Initialize the building components for the Inverse Wishart Distribution
  Matrix C1(3,3,0.0);
  double C=0, C2=0, C3=0;

  Matrix tmp_P(5,5,0.0), weighted_tmp_P(5,5,0.0);
  
  Matrix tmp_V(d,d,0.0);
  
  Matrix eye = Matrix::identity(d);
  
  double w, alpha, beta, v, sum;
  Matrix V(3,3,0.0), tempV(3,3,0.0);
  objStateSpace tmpObj;

  // weighted sum for alpha
  double a_k = 0, tmpBeta = 0;
  for(int i = 0; i<newMBs->size(); i++){
    w = PPP_weights->at(i);
    
    //std::cout << newMBs->at(i).X;
    mergedComponents.X.x += norm_w*newMBs->at(i).X.x*w;
    mergedComponents.X.y += norm_w*newMBs->at(i).X.y*w;
    mergedComponents.X.v += norm_w*newMBs->at(i).X.v*w;
    mergedComponents.X.theta += norm_w*newMBs->at(i).X.theta*w;
    mergedComponents.X.w += norm_w*newMBs->at(i).X.w*w;

    Matrix tmpObjStateSpaceDiff(newMBs->at(i).X - mergedComponents.X);

    tmp_P = newMBs->at(i).P + tmpObjStateSpaceDiff*tmpObjStateSpaceDiff.transpose();
    weighted_tmp_P = w*tmp_P;
    mergedComponents.P = mergedComponents.P + weighted_tmp_P;

    v = newMBs->at(i).v;
    tempV = newMBs->at(i).V;
    V = V + w*(v -d -1)*tempV.inv();


    // Gamma components
    alpha = newMBs->at(i).alpha;
    beta = newMBs->at(i).beta;
    double dgamma = polygamma(0,alpha);
    c1 += w*(dgamma-log(beta));
    c2 += w*alpha/beta;

    // IW components
    C1 = C1 + w*(v-d-1)*V*eye.inv();
    sum = 0;
    for(int j = 1;j<=d;j++){
      sum += polygamma(0,(v-d-j)/2);
    }

    C2 += w*sum;
    C3 += w*log(V.determinant());
    
    // Weigheted sum for alpha
    a_k += w*newMBs->at(i).alpha/w_bar;
    tmpBeta += a_k/newMBs->at(i).beta;
  }
  mergedComponents.P = norm_w*mergedComponents.P;

  // Equation to find alpha in the Gamma distribution
  c = norm_w*c1 - log(c2*norm_w);

  // Equation to find the merged DOF (v)
  C = d*w_bar*log(w_bar) - w_bar*log(C1.determinant()) + C2 -C3;

  // Iteration to find alpha using Halleys solving method  
  int k=0;
  double h_k, hp_k, hb_k, a_new;
  while(k < 100){
    k++;
    h_k = log(a_k) - polygamma(0,a_k) + c;
    hp_k = 1/a_k - polygamma(1,a_k);
    hb_k = -1/pow(a_k,2) -polygamma(2,a_k);
    
    // Halley's equation
    a_new = a_k-(2*h_k*hp_k)/(2*pow(hp_k,w)-h_k*hb_k);
    if(abs(a_new-a_k)<1e-2){
      a_k = a_new;
      break;
    }else{
      a_k = a_new;
    }
    a_k = std::max(a_k, a_new);
  }
  if(a_k != a_k){
    throw std::runtime_error("Merged alpha is NaN.");
  }
  mergedComponents.alpha = a_k;
  mergedComponents.beta = a_k/tmpBeta;

  h_k = 0;
  hb_k = 0;
  hp_k = 0;

  double v_new;
  double v_k = std::accumulate(newMBs->begin(), newMBs->end(), 0.0, [](double sum, const TrackedObj& obj) {
      return sum + obj.v;
  });
  v_k = v_k/newMBs->size();

  k = 0;
  double sum0=0, sum1=0, sum2=0;
  
  // Iteration to find v using Halleys
  while(k<100){
    k++;
    sum0 = 0;
    sum1 = 0;
    sum2 = 0;

    for(int j = 1;j<=d;j++){
      sum0 += polygamma(0,(v_k-d-j)/2);
      sum1 += polygamma(1,(v_k-d-j)/2);
      sum2 += polygamma(2,(v_k-d-j)/2);
    }
    h_k = d*w_bar*log(v_k-d-1)-w_bar*sum0 + C;
    hp_k = d*w_bar/(v_k-d-1) -0.5*w_bar*sum1;
    hb_k = -d*w_bar/(pow(v_k-d-1,2)-0.25*w_bar*sum2);

    v_new = v_k-(2*h_k*hp_k)/(2*pow(hp_k,2)-h_k*hb_k);

    if(abs(v_new-v_k)<1e-2){
      v_k = v_new;
      break;
    }else{
      v_k = v_new;
    }
    v_k = std::max(v_k, v_new);
  }
  if(v_k != v_k){
    throw std::runtime_error("Merged v is NaN.");
  }
  mergedComponents.v = v_k;
  mergedComponents.V = w_bar*(mergedComponents.v-d-1)*V.inv();

  return mergedComponents;
};