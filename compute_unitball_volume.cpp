#include "compute_unitball_volume.hpp"
#include "MontecarloIntegrate.hpp"
#include <TGraph.h>
#include <TCanvas.h>
#include <TF1.h> 
#include <vector> 
#include <cstdio> 
#include <iostream> 

using namespace std; 

void compute_unitball_volume(const int n_dim_min, const int n_dim_max, const char* path_png, const long unsigned int integration_pts)  
{
    vector<double> pts_dimension, pts_vol;

    for (int dim = n_dim_min; dim<=n_dim_max; dim++) {

        //create our bounding box as the 'unit' cube (with s=2)
        vector<IntegrationBound_t> bounds;
        for (int d=0; d<dim; d++) bounds.push_back({-1., 1.}); 
        
        //define our fcn, which returns 'true' when inside the unit sphere
        auto is_in_ball = [dim](const double* X) 
        {
            double val=0.; 
            for (int i=0; i<dim; i++) val += X[i]*X[i]; 
            return val < 1. ? true : false; 
        };
        
        auto volume = MontecarloIntegrate(integration_pts, bounds, is_in_ball);

        pts_dimension.push_back((double)dim); 
        pts_vol      .push_back(volume.val); 
    
        printf(" %2i/%i - vol: %.6f +/- %.6f\n", dim, n_dim_max, volume.val, volume.error);
        cout << flush; 
    }

    auto graph = new TGraph(pts_dimension.size(), pts_dimension.data(), pts_vol.data()); 

    auto tf1 = new TF1("volume", "pow(TMath::Pi(), x/2.)/ROOT::Math::tgamma(x/2. + 1.)", n_dim_min, n_dim_max); 

    auto canv = new TCanvas; 

    //gPad->SetLogy(1); 
    
    graph->SetTitle("Volume of unit (R=1) N-ball;dimension;volume"); 

    graph->SetLineStyle(kSolid); 
    graph->SetMarkerStyle(kOpenCircle); 
    graph->Draw();

    tf1->SetLineWidth(2);
    tf1->SetLineStyle(kDashed); 
    tf1->Draw("SAME"); 

    canv->SaveAs("nball_volume.png"); 
}