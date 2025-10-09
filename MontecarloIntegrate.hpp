#ifndef MontecarloIntegrate_H
#define MontecarloIntegrate_H

#include <optional> 
#include <array> 
#include <limits> 
#include <functional> 
#include <vector> 
#include "ValueWithError.hpp"

// A generalized monte-carlo integration tool 

// tells you what your integration bounds are
struct IntegrationBound_t {
    double xmin{-1.}, xmax{1.};
};

ValueWithError_t<double> MontecarloIntegrate(
    const unsigned long int n_pts,                  //number of points to use in the integration 
    const std::vector<IntegrationBound_t> bounds,   //number of dimensions is given by the number of bounds given.  
    std::function<bool(const double*)> fcn          //fcn to integrate. must accept (CONST) ptr to doubles.  
); 

#endif