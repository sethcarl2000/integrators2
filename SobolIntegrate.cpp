#include "SobolIntegrate.hpp"
#include <random> 
#include <chrono> 
#include <iostream> 
#include <thread>
#include <Math/QuasiRandom.h>
#include <chrono>
#include <random> 

using namespace std; 
using QRS = ROOT::Math::QuasiRandomSobol; 

// A generalized monte-carlo integration tool 

//this is kinda a hacky way to do this, but its how we can have our 
// generators skip an aribtrary number of 'pairs' in the sequence each time, 
// without waisting any. 
static std::map<int, std::unique_ptr<ROOT::Math::QuasiRandomSobol>> sobol_generators{}; 


ValueWithError_t<double> SobolIntegrate(
    const unsigned long int n_pts,                  //number of points to use in the integration. 
                                                    // for example, if you choose npts=100 and dim=5, then total points is 100^5. 
    const std::vector<IntegrationBound_t> bounds,   //number of dimensions is given by the number of bounds given. 
    std::function<bool(const double*)> fcn          //fcn to integrate. must accept (CONST) ptr to doubles. returns TRUE if inside region, FALSE if not.               
)
{ 
    //dimension of the space we're integrating in 
    const int dim = (int)bounds.size(); 

    //look to see if a sobol generator of this dimension already exists 
    auto it = sobol_generators.find(dim); 

    QRS* sobol = nullptr; 

    if (it == sobol_generators.end()) {
        //we need to make a new one; none exists yet 
        sobol_generators[dim] = unique_ptr<QRS>(
            new QRS(dim)
        ); 
        
        sobol = sobol_generators[dim].get(); 
    } else {
        //one already exists; let's use it    
        sobol = it->second.get(); 
    }   



    //create an array of ints, which represents the 'point' in space we're using. 
    vector<double> point(dim, 0.);

    unsigned long long count =0; 
    
    for (unsigned long int i=0; i<n_pts; i++) {

        sobol->Next(point.data()); 

        //check the fcn at our current Sobol point
        if (fcn(point.data())) count++;    
    }

    //compute the volume of our 'box' we're integrating in 
    double total_vol{1.}; 
    for (auto bound : bounds) total_vol *= (bound.xmax - bound.xmin);

    double result = total_vol * (((double)count) / ((double)n_pts)); 
    //very rudimentary error estimate
    double error  = total_vol * (sqrt((double)count) / ((double)n_pts)); 

    return ValueWithError_t<double>{ result, error }; 
}

