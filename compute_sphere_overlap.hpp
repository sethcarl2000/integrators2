#ifndef compute_sphere_overlap_H
#define comptue_sphere_overlap_H

#include "ValueWithError.hpp"

// args - 
//  -   dimension dimension of the space the spheres live in 
//  -   number of monte-carlo points to use 
//  -   radius of sphere 1 
//  -   radius of sphere 2 
//  -   separation 
ValueWithError_t<double> compute_sphere_overlap(
    const int dimenison, 
    const long unsigned int N, 
    const double R1, 
    const double R2, 
    const double sep 
); 

#endif 