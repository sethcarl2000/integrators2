#ifndef GridIntegrate_H
#define GridIntegrate_H

#include <optional> 
#include <array> 
#include <limits> 
#include <functional> 
#include <vector> 
#include "ValueWithError.hpp"
#include "IntegrationBound.hpp"

// A generalized monte-carlo integration tool 

ValueWithError_t<double> GridIntegrate(
    const long unsigned int n_pts_per_side,         //number of points PER SIDE of the n-hypercube to use 
    const std::vector<IntegrationBound_t> bounds,   //number of dimensions is given by the number of bounds given.  
    std::function<bool(const double*)> fcn          //fcn to integrate. must accept (CONST) ptr to doubles.  
); 

#endif