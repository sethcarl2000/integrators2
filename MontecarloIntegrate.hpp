#ifndef MontecarloIntegrate_H
#define MontecarloIntegrate_H

#include <optional> 
#include <array> 
#include <limits> 
#include <functional> 
#include <vector> 

// A generalized monte-carlo integration tool 

// tells you what your integration bounds are
struct IntegrationBound_t {
    double xmin{-1.}, xmax{1.};
};

// kind-of like std::optional. 
struct IntegrationResult_t{

    bool is_valid{false}; 
    double 
        val{std::numeric_limits<double>::quiet_NaN()},
        error{std::numeric_limits<double>::quiet_NaN()};  

    //implicit conversion to bool, to check if result is valid. like:
    //
    // >    auto result = MontecarloIntegrator(...);
    // >  
    // >    if (result) { // means: is_valid == true
    // >        ... do stuff ... 
    // >    } else {      // means: is_valid == false
    // >        ... other stuff ... 
    // >    }
    // 
    operator bool() const { return is_valid; } 

    //implicit conversion to double 
    operator double() const { 
        if (is_valid) { return val; }
        else          { return std::numeric_limits<double>::quiet_NaN(); } 
    }
}; 

IntegrationResult_t MontecarloIntegrate(
    const unsigned long int n_pts,                  //number of points to use in the integration 
    const std::vector<IntegrationBound_t> bounds,   //number of dimensions is given by the number of bounds given.  
    std::function<bool(const double*)> fcn          //fcn to integrate. must accept (CONST) ptr to doubles.  
); 




#endif