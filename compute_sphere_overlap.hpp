#ifndef compute_sphere_overlap_H
#define comptue_sphere_overlap_H

#include "ValueWithError.hpp"
#include "MontecarloIntegrate.hpp"
#include <functional>

// args - 
//  -   dimension dimension of the space the spheres live in 
//  -   number of monte-carlo points to use 
//  -   radius of sphere 1 
//  -   radius of sphere 2 
//  -   separation 
enum IntegratorType { 
    kMontecarlo     = 1,
    kQuasirandom    = 2,
    kGrid           = 3
};

ValueWithError_t<double> compute_sphere_overlap(
    const int dimenison, 
    const long unsigned int N, 
    const double R1, 
    const double R2, 
    const double sep, 
    IntegratorType integrator_type=kMontecarlo
); 

#endif 