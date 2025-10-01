#include <MontecarloIntegrate.hpp> 
#include <random> 
#include <chrono> 
#include <iostream> 

using namespace std; 

IntegrationResult_t MontecarloIntegrate(
    //number of points to use in the integration 
    const unsigned long int n_pts,
    //number of dimensions is given by the number of bounds given. 
    const std::vector<IntegrationBound_t> bounds,
    //fcn to integrate. must accept (CONST) ptr to doubles. returns TRUE if inside region, FALSE if not.               
    std::function<bool(const double*)> fcn
)
{   
    //dimension of the space we're integrating in 
    const int dim = (int)bounds.size(); 
    
    //this will seed our random-number generators 
    random_device rd; 

    mt19937 mtengine(rd());  

    auto dist = uniform_real_distribution<double>(0., 1.); 
    auto get_uniform = [&dist, &mtengine](const IntegrationBound_t& bound) {
        return bound.xmin + (bound.xmax - bound.xmin)*dist(mtengine);
    };

    vector<double> test_X; 
    for (const auto& bound : bounds) test_X.push_back( get_uniform(bound) );
 
    
    //now, actually do the integration        
    unsigned long int inside=0; 

    unsigned long int i=0; 
    while (i++ < n_pts) {

        if (fcn(test_X.data())) inside++;

        for (int i=0; i<dim; i++) test_X[i] = get_uniform(bounds[i]); 
    }

    //compute the volume of our 'box' we're integrating in 
    double total_vol{1.}; 
    for (auto bound : bounds) total_vol *= (bound.xmax - bound.xmin);

    double result = total_vol * ((double)inside) / ((double)n_pts); 
    //very rudimentary error estimate
    double error  = total_vol * (sqrt((double)inside) / ((double)n_pts)); 

    return IntegrationResult_t{ .is_valid=true, .val=result, .error=error }; 
}

