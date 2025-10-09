#include <MontecarloIntegrate.hpp> 
#include "compute_unitball_volume.hpp"
#include "compute_sphere_overlap.hpp"
#include <cmath> 
#include <iostream>
#include <TGraph.h> 
#include <TCanvas.h>  
#include <cstdio> 
#include <TF1.h> 
#include <TLegend.h> 
#include <TPad.h> 
#include <map> 
#include <vector> 

using namespace std; 

int main(int argc, char* argv[])
{   
    //take in the arg list, with default values.
    int i_arg=1;

    int dim             = argc > i_arg ? atoi(argv[i_arg++])    :   3; 
    long unsigned int N = argc > i_arg ? atof(argv[i_arg++])    :   1e6; 
    double rad0         = argc > i_arg ? atof(argv[i_arg++])    :   1.; 
    double rad1         = argc > i_arg ? atof(argv[i_arg++])    :   1.; 
    double sep          = argc > i_arg ? atof(argv[i_arg++])    :   0.; 

    printf(
        "dim    = %i\n"
        "N pts. = %li\n"
        "r1     = %.5f\n"
        "r0     = %.5f\n"
        "sep    = %.5f\n",

        dim, N, rad0, rad1, sep
    );

    cout << "computing..." << flush; 

    auto result = compute_sphere_overlap(dim, N, rad0, rad1, sep); 

    double vol = result.val;
    double err = result.error; 

    printf("done\nvolume of intersection: %.6e +/- %.3e\n", vol, err);

    return 0; 
}