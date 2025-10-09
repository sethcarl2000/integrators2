#include "GridIntegrate.hpp"
#include <random> 
#include <chrono> 
#include <iostream> 
#include <thread>

using namespace std; 

ValueWithError_t<double> GridIntegrate(
    const unsigned long int n_pts,                  //number of points to use in the integration PER SIDE. 
                                                    // for example, if you choose npts=100 and dim=5, then total points is 100^5. 
    const std::vector<IntegrationBound_t> bounds,   //number of dimensions is given by the number of bounds given. 
    std::function<bool(const double*)> fcn          //fcn to integrate. must accept (CONST) ptr to doubles. returns TRUE if inside region, FALSE if not.               
)
{ 
    //dimension of the space we're integrating in 
    const int dim = (int)bounds.size(); 

    //get the number of threads we have to work with
    const int n_threads = std::thread::hardware_concurrency(); 

    //create an array of ints, which represents the 'point' in space we're using. 
    vector<int> point_id(dim, 0); 
    vector<double> point;
    vector<double> dx; 

    //start at one 'corner' of our hypercube. 
    for (int i=0; i<dim; i++) {

        //starting point
        point.push_back(bounds[i].xmin);  

        //grid spacing
        dx.push_back( (bounds[i].xmax - bounds[i].xmin)/((double)n_pts-1) ); 
    }

    unsigned long long count =0; 
    
    while (1) {

        //check the fcn at our current grid point
        if (fcn(point.data())) count++;
        
        //now, update our point
        bool at_end=true; 
        for (int i=0; i<dim; i++) {

            point_id[i]++; 
            point[i]    = bounds[i].xmin + ( dx[i] * ((double)point_id[i]) );

            //check if this point is out-of-bounds. if not, then we're ok to increment it and move on. 
            if (point_id[i] < n_pts) { at_end=false; break; }

            //if we got here, that means that we've reached the end of the hyper-grid
            // w/r/t this coordinate. we need to increment the next one...
            
            // reset this coordinate, and continue by iterating the next one. 
            point_id[i] = 0; 
            point[i]    = bounds[i].xmin;
            
        }
        if (at_end) break; 
        
       //printf("point: %+.2f %+.2f %+.2f\n", point[0], point[1], point[2]);
    }

    //compute the volume of our 'box' we're integrating in 
    double total_vol{1.}; 
    for (auto bound : bounds) total_vol *= (bound.xmax - bound.xmin);

    double result = total_vol * (((double)count) / pow( n_pts, dim )); 
    //very rudimentary error estimate
    double error  = total_vol * (sqrt((double)count) / pow( n_pts, dim )); 

    return ValueWithError_t<double>{ result, error }; 
}

