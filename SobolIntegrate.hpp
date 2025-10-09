#ifndef SobolIntegrate_H
#define SobolIntegrate_H

#include <optional> 
#include <array> 
#include <limits> 
#include <functional> 
#include <vector> 
#include "ValueWithError.hpp"
#include "IntegrationBound.hpp"

// A generalized monte-carlo integration tool 

ValueWithError_t<double> SobolIntegrate(
    const long unsigned int npts,                   //number of points to use in the quasai-random sequence 
    const std::vector<IntegrationBound_t> bounds,   //number of dimensions is given by the number of bounds given.  
    std::function<bool(const double*)> fcn          //fcn to integrate. must accept (CONST) ptr to doubles.  
); 

#endif