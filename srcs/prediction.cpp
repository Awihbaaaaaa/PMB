/* #include "../Headers/obj.hpp"
#include <cmath>
#include "../Headers/ExtendedObjectDefinition.hpp" */
#include "../Headers/declarations.hpp"

void predictPPP(std::vector<TrackedObj>& PPP, ExtendedObjectDefinition extendedObjDef);
void predictMB(std::vector<TrackedObj>& PPP, ExtendedObjectDefinition extendedObjDef);

/**
 * @brief Predict the state evolution of untracked objects using the Poisson Point Process (PPP) model.
 *
 * This function updates the state estimates of untracked objects using the Poisson Point Process (PPP) model.
 * The predictions consider the specified motion model, extension dimension, shape uncertainty, and survival probability.
 *
 * @param PPP A vector of UntrackedObj representing untracked objects.
 * @param extendedObjDef An ExtendedObjectDefinition defining the motion model and other parameters.
 */
void predictPPP(std::vector<UntrackedObj>& PPP, ExtendedObjectDefinition extendedObjDef){
    // Extract motion model, extension dimension, and shape uncertainty from ExtendedObjectDefinition
    ctModel model = extendedObjDef.getMotionModel();
    double d = extendedObjDef.getExtensionDimension();
    double eta = extendedObjDef.getShapeUncertainty();
    
    // Check if there are objects in the PPP set    
    if(PPP[0].w_ppp){
        // Iterate through each UntrackedObj in the PPP set
        for(UntrackedObj& obj : PPP){
            // Update probability of existence considering survival probability
            obj.w_ppp = obj.w_ppp*extendedObjDef.getSurvivalProbability();

            // Scale alpha and beta parameters based on shape uncertainty
            obj.alpha = obj.alpha/eta;
            obj.beta = obj.beta/eta;

            // Update state using motion model
            model.f(obj.X);

            // Predicted covariance update based on motion model and motion covariance
            obj.P = model.F(obj.X)*obj.P*model.F(obj.X).transpose() + model.Q();

            // Update the inverse wishart dof using shape uncertainty
            obj.v = 2*d + 2 + exp(-model.getT()/eta)*(obj.v-2*d-2);

            // Update shape covariance matrix using shape uncertainty and rotation
            obj.V = exp(-model.getT()/eta)*extendedObjDef.rotationMatrix(&obj.X)*obj.V*extendedObjDef.rotationMatrix(&obj.X).transpose();
        }
    }
}

/**
 * @brief Predict the state evolution of tracked objects using the Multi Bernoulli (MB) model.
 *
 * This function updates the state estimates of tracked objects using the Multi Bernoulli (MB) model.
 * The predictions consider the specified motion model, extension dimension, shape uncertainty, and survival probability.
 *
 * @param MB A vector of TrackedObj representing tracked objects.
 * @param extendedObjDef An ExtendedObjectDefinition defining the motion model and other parameters.
 */
void predictMB(std::vector<TrackedObj>& MB, ExtendedObjectDefinition extendedObjDef){
    // Extract motion model, extension dimension, and shape uncertainty from ExtendedObjectDefinition
    ctModel model = extendedObjDef.getMotionModel();
    double d = extendedObjDef.getExtensionDimension();
    double eta = extendedObjDef.getShapeUncertainty();
    
    // Check if there are objects in the MB set
    if(MB[0].r_MB){
        // Iterate through each TrackedObj in the MB set
        for(TrackedObj& obj:MB){
            // Update probability of existence considering survival probability
            obj.r_MB = extendedObjDef.getSurvivalProbability()*obj.r_MB;

            // Scale alpha and beta parameters based on shape uncertainty
            obj.alpha = obj.alpha/eta;
            obj.beta = obj.beta/eta;

            // Update state using motion model
            model.f(obj.X);

            // Predicted covariance update
            obj.P = model.F(obj.X)*obj.P*model.F(obj.X).transpose() + model.Q();

            // Update the inverse wishart dof using shape uncertainty
            obj.v = 2*d + 2 + exp(-model.getT()/eta)*(obj.v-2*d-2);

            // Update covariance matrix using shape uncertainty and rotation
            obj.V = exp(-model.getT()/eta)*extendedObjDef.rotationMatrix(&obj.X)*obj.V*extendedObjDef.rotationMatrix(&obj.X).transpose();
        }
    }
}

/**
 * @brief Predict the state evolution of collect of objects using the Poisson Point Process (PPP) and Multi Bernoulli (MB) models.
 *
 * This function updates the state estimates of collect of objects using the Poisson Point Process (PPP) and Multi Bernoulli (MB) models.
 * The predictions consider the specified motion model, extension dimension, shape uncertainty, and survival probability.
 *
 * @param CurrentObjectsCollection An objects collection including two list of tracked and untracked objects.
 * @param extendedObjDef An ExtendedObjectDefinition defining the motion model and other parameters.
 */
void predict(ObjectsCollection& CurrentObjectsCollection, ExtendedObjectDefinition extendedObjDef){
    predictPPP(CurrentObjectsCollection.PPP, extendedObjDef);
    predictMB(CurrentObjectsCollection.MB, extendedObjDef);
}


