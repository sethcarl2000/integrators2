#include <MontecarloIntegrate.hpp> 
#include "compute_unitball_volume.hpp"
#include <cmath> 
#include <iostream>
#include <TGraph.h> 
#include <TCanvas.h>  
#include <cstdio> 
#include <TF1.h> 
#include <TLegend.h> 
#include <TPad.h> 

using namespace std; 

int main(int argc, char* argv[])
{   
    compute_unitball_volume(2, 15, "nball_volume.png"); 
    
    return 0; 
}