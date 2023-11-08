#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "../Headers/obj.hpp"


ObjectsCollection initPPP(const double tot_exp_objs,
                        const double objs_rate,
                        const double dof); 

namespace py = pybind11;

py::object retrieveObjects(double tot_exp_objs, double objs_rate, double dof) {
    ObjectsCollection collection = initPPP(tot_exp_objs, objs_rate, dof);
    std::vector<double> tempVec;
    
    py::list CamRad_PPP;

    for(const UntrackedObj& obj: collection.PPP){
        py::dict objDict;
        tempVec = {obj.X.x, obj.X.y, obj.X.v, obj.X.theta, obj.X.w};
        objDict["x"] = py::cast(tempVec);
        objDict["P"] = py::cast(obj.P.getData());  
        objDict["v"] = py::cast(obj.v);
        objDict["V"] = py::cast(obj.V.getData());
        objDict["alpha"] = py::cast(obj.alpha);
        objDict["beta"] = py::cast(obj.beta);
        objDict["w"] = obj.w_ppp;

        CamRad_PPP.append(objDict);
    }

    // Convert collection to a Python object and return it
    return CamRad_PPP;
}

PYBIND11_MODULE(my_module, m) {
    m.def("retrieveObjects", &retrieveObjects, "Initialize PPP collection");
}


