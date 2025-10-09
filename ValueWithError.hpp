#ifndef ValueWithError_H
#define ValueWithError_H

#include <limits> 

// A very simple struct which returns a value with an error, which can be 
// implicitly converted to type 'T'.

template<typename T> struct ValueWithError_t { 
    T val  {std::numeric_limits<double>::quiet_NaN()}; 
    T error{std::numeric_limits<double>::quiet_NaN()}; 
    
    operator T() const { return val; }
};

#endif 