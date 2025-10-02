#include <MontecarloIntegrate.hpp> 
#include <random> 
#include <chrono> 
#include <iostream> 
#include <thread>

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

    //get the number of threads we have to work with
    const int n_threads = std::thread::hardware_concurrency(); 

    //initialize the vector of sub-results
    vector<unsigned long int> sub_counts(n_threads); 

    //this way, we will have **just over** n_pts pts total
    const int n_pts_per_thread = (n_pts / n_threads) + 1;

    //make the vector of threads
    vector<thread> threads; 

    for (int t=0; t<n_threads; t++) {

        //now, with each thread, actually compute the result
        threads.emplace_back([dim, n_pts_per_thread, t, &sub_counts, fcn, bounds]{

            //grab a reference to this specific sub-count
            unsigned long int& sub_count = sub_counts[t];  

            //this is done here just for readability, even though the u.l.I type should
            // by default be initialized to 0. 
            sub_count = 0; 
            
            //  --- now, actually compute the volume by picking random points --- 
            //this will seed our random-number generators 
            random_device rd; 

            mt19937 mtengine(rd());  

            auto dist = uniform_real_distribution<double>(0., 1.); 
            auto get_uniform = [&dist, &mtengine](const IntegrationBound_t& bound) {
                return bound.xmin + (bound.xmax - bound.xmin)*dist(mtengine);
            };

            //this is our vector which is a random point in our rectangular sub-space 
            vector<double> space_point(dim); 
            
            unsigned long int i=0; 
            while (i++ < n_pts_per_thread) {

                int j=0; 
                for (const auto& bound : bounds) space_point[j++] = get_uniform(bound);

                if (fcn(space_point.data())) sub_count++;
            }
        });
    }

    //this tells the program to wait until all threads are done executing. 
    for (auto& thread : threads) thread.join(); 

    //add all the sub-results together
    unsigned long int count = 0; 
    for (auto sub_count : sub_counts) count += sub_count;

    //compute the volume of our 'box' we're integrating in 
    double total_vol{1.}; 
    for (auto bound : bounds) total_vol *= (bound.xmax - bound.xmin);

    double result = total_vol * ((double)count) / ((double)n_pts); 
    //very rudimentary error estimate
    double error  = total_vol * (sqrt((double)count) / ((double)n_pts)); 

    return IntegrationResult_t{ .is_valid=true, .val=result, .error=error }; 
}

