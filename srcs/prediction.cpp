/* #include "../Headers/obj.hpp"
#include <cmath>
#include "../Headers/ExtendedObjectDefinition.hpp" */
#include "../Headers/declarations.hpp"

/* Matrix operator*(const double scaler, const Matrix& matrix){
    Matrix res(matrix.nrRows(), matrix.nrCols());

    for(int i = 0; i < matrix.nrRows(); i++){
        for(int j = 0; j < matrix.nrCols(); j++){
            res(i,j) = scaler * matrix(i,j);
        }
    }
    return res;
}
 */
void predictPPP(std::vector<TrackedObj>& PPP, ExtendedObjectDefinition extendedObjDef);
void predictMB(std::vector<TrackedObj>& PPP, ExtendedObjectDefinition extendedObjDef);

void predictPPP(std::vector<UntrackedObj>& PPP, ExtendedObjectDefinition extendedObjDef){
    ctModel model = extendedObjDef.getMotionModel();
    double d = extendedObjDef.getExtensionDimension();
    double eta = extendedObjDef.getShapeUncertainty();
    
    if(PPP[0].w_ppp){
        for(UntrackedObj& obj : PPP){
            obj.w_ppp = obj.w_ppp*extendedObjDef.getSurvivalProbability();
            obj.alpha = obj.alpha/eta;
            obj.beta = obj.beta/eta;
            model.f(obj.X);
            obj.P = model.F(obj.X)*obj.P*model.F(obj.X).transpose() + model.Q();
            obj.v = 2*d + 2 + exp(-model.getT()/eta)*(obj.v-2*d-2);
            obj.V = exp(-model.getT()/eta)*extendedObjDef.rotationMatrix(&obj.X)*obj.V*extendedObjDef.rotationMatrix(&obj.X).transpose();
        }
    }
}

void predictMB(std::vector<TrackedObj>& MB, ExtendedObjectDefinition extendedObjDef){
    ctModel model = extendedObjDef.getMotionModel();
    double d = extendedObjDef.getExtensionDimension();
    double eta = extendedObjDef.getShapeUncertainty();
    
    if(MB[0].r_MB){
        for(TrackedObj& obj:MB){
            obj.r_MB = extendedObjDef.getSurvivalProbability()*obj.r_MB;
            obj.alpha = obj.alpha/eta;
            obj.beta = obj.beta/eta;
            model.f(obj.X);
            obj.P = model.F(obj.X)*obj.P*model.F(obj.X).transpose() + model.Q();
            obj.v = 2*d + 2 + exp(-model.getT()/eta)*(obj.v-2*d-2);
            obj.V = exp(-model.getT()/eta)*extendedObjDef.rotationMatrix(&obj.X)*obj.V*extendedObjDef.rotationMatrix(&obj.X).transpose();
        }
    }
}

void predict(ObjectsCollection& CurrentObjectsCollection, ExtendedObjectDefinition extendedObjDef){
    predictPPP(CurrentObjectsCollection.PPP, extendedObjDef);
    predictMB(CurrentObjectsCollection.MB, extendedObjDef);
}


