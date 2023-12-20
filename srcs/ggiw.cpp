#include <vector>
#include <tgmath.h>
#include "../Headers/declarations.hpp"

/**
 * @brief Perform a Gaussian-Gamma Inverse Wishart (GGIW) update on a single object.
 *
 * The function updates the state estimate of an object state space using a joint distribution called Gaussian-Gamma Inverse Wishart (GGIW).
 *
 * @param obj Pointer to the untracked object to be updated.
 * @param z Pointer to the measurements associated with the object.
 * @param radar Pointer to the radar definition used for measurements.
 * @param object Pointer to the extended object definition.
 *
 * @return A structure of type GGIW_result, containing the log-likelihood and the updated state estimate.
 *//* 
template <typename T>
GGIW_result ggiwUpdate(T* obj,
                measurements* z,
                radarDefinition* radar,
                ExtendedObjectDefinition* object){
    TrackedObj tempObj;

    int m = z->z->nrCols();
    double scalingFactor = 0.25;
    int d = object->getExtensionDimension();
    Matrix R = radar->getMeasurementsCovariance();
    Matrix H = radar->measurementModel(&(obj->X));
   
    // Gamma distribution update
    tempObj.alpha = obj->alpha + m;
    tempObj.beta = obj->beta + 1;

    Matrix x_hat_temp = obj->V*(1/(obj->v-2*d-2));
    Matrix x_hat = (x_hat_temp+x_hat_temp.transpose())/2;
    Matrix R_hat = x_hat*scalingFactor + R;

    // Innovation covariance update
    Matrix S_temp = H*obj->P*H.transpose() + R_hat/m;
    Matrix S = (S_temp+S_temp.transpose())/2;

    // Kalman gain
    Matrix K = obj->P*H.transpose()*S.inv();

    Matrix z_bar = z->z->mean(2);

    // Innovation measurement matrix
    Matrix eps = z_bar - H*obj->X;

    tempObj.X = obj->X + K*eps;

    tempObj.P = obj->P - K*H*obj->P;

    tempObj.v = obj->v + m;

    Matrix s_chol = S.chol();

    Matrix s_chol_transposed = s_chol.transpose();
    Matrix s_chol_transposed_inv = s_chol_transposed.inv();

    Matrix x_hat_chol = x_hat.chol();
    Matrix x_hat_chol_transposed = x_hat_chol.transpose();
    Matrix x_hat_chol_transposed_inv = x_hat_chol_transposed.inv();

    Matrix N = x_hat_chol*s_chol.inv()*(eps*eps.transpose())*s_chol_transposed_inv*x_hat_chol_transposed_inv;

    Matrix z_diff = *(z->z)-z_bar;
    Matrix Z = z_diff*z_diff.transpose();

    Matrix R_hat_chol = R_hat.chol();
    Matrix R_hat_chol_trans = R_hat_chol.transpose();
    Matrix R_hat_chol_trans_inv = R_hat_chol_trans.inv();

    Matrix z_hat = x_hat_chol*R_hat_chol.inv()*Z*R_hat_chol_trans_inv*x_hat_chol_transposed;
    tempObj.V = obj->V+N+z_hat;

    double l1,l2,l3,l4,l5,l6;
    double obj_v_mean = obj->v-d-1;
    double update_v_mean = tempObj.v-d-1;

    l1 = -0.5*d*m*log(M_PI)-0.5*d*log(m);

    double V_det = obj->V.determinant();
    l2 = 0.5*(obj_v_mean)*log(V_det)-0.5*(update_v_mean)*log(tempObj.V.determinant());

    double b_obj = 2*log(M_PI) + lgamma(obj_v_mean/2) + lgamma(obj_v_mean/2-0.5);
    double b_update = 2*log(M_PI) + lgamma(update_v_mean/2) + lgamma(update_v_mean/2-0.5);
    l3 = b_update - b_obj;

    l4 = m*0.5*log(x_hat.determinant())-(m-1)*0.5*log(R_hat.determinant())-0.5*log(S.determinant());

    l5 = lgamma(tempObj.alpha)-lgamma(obj->alpha);

    l6 = obj->alpha*log(obj->beta)-tempObj.alpha*log(tempObj.beta);

    GGIW_result result;
    result.L = l1+l2+l3+l4+l5+l6;
    result.newMB = tempObj;

    return result;
} */