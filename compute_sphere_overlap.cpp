#include <cmath> 
#include <vector> 
#include <stdexcept> 
#include <sstream> 

#include "compute_sphere_overlap.hpp"
#include "ValueWithError.hpp"

//integrators we can use
#include "MontecarloIntegrate.hpp"
#include "SobolIntegrate.hpp"
#include "GridIntegrate.hpp"

using namespace std; 


ValueWithError_t<double> compute_sphere_overlap(
    const int dimenison, 
    const long unsigned int N, 
    const double R1, 
    const double R2, 
    const double sep,
    IntegratorType integrator_type
) 
{   
    //check some basic constraints
    if (!(R1 > 0. && R2 > 0. && sep >= 0. && R1 >= R2)) {
        ostringstream oss; 
        oss << "in <compute_spheres_overlap>: R1 (" << R1 << "), R1 (" << R2 
            << "), or sep (" << sep << ") is invalid; they must all be positive, and R1 >= R2!";    
        throw invalid_argument(oss.str()); 
        return ValueWithError_t<double>{}; 
    } 

    //create the function that returns 'true' if a given point is within both spheres 
    //taking sphere 1 to be at the origin, and sphere 2 to have its center at: 
    //
    // \vec{R}_2 = {sep,0,...,0}. 
    //
    const double R1_R1 = R1*R1; 
    const double R2_R2 = R2*R2;
    
    //_______________________________________________________________________________
    auto is_inside_both_spheres = [R1_R1,R2_R2,sep,dimenison](const double* X) 
    {   
        //check the first sphere
        double val=0.; 
        for (int i=0; i<dimenison; i++) val += X[i]*X[i];

        //check if outside of the first sphere
        if (val > R1_R1) return false; 
    
        //compute dist to second sphere
        // we can do this little trick because this is the only difference between the 
        // distance to the two spheres 
        val += (( X[0] - sep )*( X[0] - sep )) - ( X[0]*X[0] ); 
        
        //check if outside of the second sphere
        if (val > R2_R2) return false; 

        //if the code makes it here, it was inside both spheres. 
        return true;
    };
    //_______________________________________________________________________________
    

    //now, make the integration bounds
    //first, for the x0-axis
    vector<IntegrationBound_t> bounds{
        { max<double>( -R1, sep - R2 ), min<double>( +R1, sep + R2 )}
    }; 
    
    //now, add bounds for all other axes
    for (int i=1; i<dimenison; i++) bounds.push_back({ -R1, R1 }); 

    //now, we are ready to do the integration 
    ValueWithError_t<double> result; 

    //check which integrator we're using
    switch (integrator_type) {
        case (kMontecarlo)  : result = MontecarloIntegrate(N, bounds, is_inside_both_spheres); break;
        case (kQuasirandom) : result = SobolIntegrate(N, bounds, is_inside_both_spheres); break; 
        case (kGrid)        : result = GridIntegrate(1 + (unsigned long int)pow(N, 1./((double)bounds.size())), bounds, is_inside_both_spheres); break; 
    }
    
    return result; 
}