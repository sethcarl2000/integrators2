#include <MontecarloIntegrate.hpp> 
#include "GridIntegrate.hpp"
#include "compute_unitball_volume.hpp"
#include "compute_sphere_overlap.hpp"
#include <cmath> 
#include <iostream>
#include <TGraph.h> 
#include <TGraphErrors.h> 
#include <TCanvas.h>  
#include <TPad.h> 
#include <cstdio> 
#include <TF1.h> 
#include <TLegend.h> 
#include <TH1F.h> 
#include <TPad.h> 
#include <map> 
#include <vector> 
#include <functional> 
#include <TLegend.h> 

using namespace std; 

//given a dimension argument 'dim', returns a function which evaluates whether
// an array double[dim] represents a point within a unit N-ball centered at the origin,
// with N = dim.
//
// for examlple: 
//  
//  auto fcn_3 = generate_inside_unitball_fcn(3); 
//  double X3d[] = {0., 0., 0.}; 
//  bool check_3d = fcn_3(X3d);        // check_3d == true
//  
//  auto fcn_4 = generate_inside_unitball_fcn(4);
//  double X4d[] = {2., 0., 0., 0.};
//  bool check_4d = fcn_4(X4d);        // check_4d == false; 
// 
function<bool(const double*)> generate_inside_unitball_fcn(const int dim)
{   
    auto fcn_dim = [dim](const double* X) 
    {
        double val=0.; 
        for (int i=0; i<dim; i++) val += X[i]*X[i]; 
        return bool(val < 1.); 
    };
    
    return fcn_dim; 
}

//return the stddev and mean of a given std::vector
void compute_vector_stddev_mean(const vector<double>& vals, double& mean, double& stddev)
{
    //quit if the vector's empty
    if (vals.empty()) return; 

    double N = (double)vals.size(); 

    //compute mean
    mean = 0.; 
    for (const auto& x : vals) mean += x; 
    mean *= 1./N; 

    //compute stddev 
    stddev = 0.; 
    for (const auto& x : vals) {
        stddev += (x - mean)*(x - mean); 
    }
    stddev = sqrt( stddev / N ); 

    return; 
} 


int main(int argc, char* argv[])
{   
    //I'm doing this so I don't have to make two different executables
    string plots_to_make = "methods";
    if (argc > 1) plots_to_make = string(argv[1]); 

    //optionally, specify the path on the command line: 
    const char* path_graphic = "test-methods.png"; 
    if (argc > 2) path_graphic = argv[2];  

    //
    cout << "making plots: " << plots_to_make << "..." << endl; 

    //make the convergence plots
    //______________________________________________________________________________________
    if (plots_to_make=="convergence") {

        const vector<int> dims = {3, 5, 10}; 

        auto canv = new TCanvas("c", "canv", 1400, 800); 
        canv->Divide(dims.size(), 2); 

        //starting number of points to integrate with
        const unsigned long int n_integ_pts_0 = 64; 

        //number of times we re-evaluate the integral for each point. 
        // the 'statistical error' will be defined as the stddev of the results 
        // for each separate 'n_integ_pts' value
        const int n_evals_per_pt = 10; 

        //each time, we will multiply by 2. 
        // so the final number of points is = n_integ_pts * pow( 2, n_eval_levels-1 )
        int n_eval_levels = 19; 

        //size of the spheres, and the offset of their centers 
        const double sphere_1_rad = 1.; 
        const double sphere_2_rad = 0.5; 
        const double sphere_sep   = 1.; 

        int i_canv=1; 
        for (const int dim : dims) {
            
            double min_y{+1.e30}, max_y{-1.e30}; 
            double max_stddev{0.};

            printf("evaluating %id sphere...\n", dim); 

            vector<double> graphPts_npts; 
            vector<double> graphPts_vol_mean;
            vector<double> graphPts_vol_stdddev;  

            unsigned long int n_integ_pts = n_integ_pts_0; 
            
            for (int i=0; i<n_eval_levels; i++) {

                //now, actually eval the integral a given number of times
                vector<double> vals{}; vals.reserve(n_evals_per_pt);  

                for (int n=0; n<n_evals_per_pt; n++) {

                    double volume = compute_sphere_overlap(
                        dim, 
                        n_integ_pts, 
                        sphere_1_rad, 
                        sphere_2_rad, 
                        sphere_sep, kGrid
                    ); 
                    vals.push_back(volume); 
                }

                //now, compute the stddev and mean
                double mean, stddev; 
                compute_vector_stddev_mean(vals, mean, stddev); 

                if (!(mean == 0. && stddev == 0.)) {
                    graphPts_npts       .push_back(sqrt(n_integ_pts)); 
                    graphPts_vol_mean   .push_back(mean); 
                    graphPts_vol_stdddev.push_back(stddev);
                } 
                
                printf("  npts = %8li, vol %.4e +/- %.4e", 
                    n_integ_pts, 
                    mean, 
                    stddev
                ); 
                cout << endl; 

                //find the min and max y-vals in the dataset 
                min_y = min<double>( mean - stddev, min_y ); 
                max_y = max<double>( mean + stddev, max_y ); 

                max_stddev = max<double>( stddev, max_stddev );


                //double the number of stone-throwing tries
                n_integ_pts *= 2; 
            }
            
            //now we're done evaluating all points. 
            vector<double> x_errors(n_eval_levels, 0.); 

            auto graph_vals = new TGraphErrors(
                graphPts_npts.size(),
                
                graphPts_npts.data(),
                graphPts_vol_mean.data(),

                x_errors.data(),
                graphPts_vol_stdddev.data()
            ); 

            auto graph_errors = new TGraph(
                graphPts_npts.size(),

                graphPts_npts.data(),
                graphPts_vol_stdddev.data()
            );

            //draw the graph of calculated volume values
            canv->cd(i_canv); 
            gPad->SetLogx(1); 
            gPad->SetLeftMargin(0.15);
            gPad->SetRightMargin(0.0); 

            //how much to extend the y-axis by (in a fraction of the total amplitude) 
            const double y_buffer = 0.1; 

            auto hist_frame = gPad->DrawFrame(
                sqrt(n_integ_pts_0 * 0.5), 
                min_y - y_buffer*(max_y - min_y),

                sqrt(n_integ_pts_0 * pow( 2, n_eval_levels ) * 2),
                max_y + y_buffer*(max_y - min_y)
            );  

            hist_frame->SetTitle(Form(
                "%id (r_{1}= %.1f, r_{2}= %.1f, a= %.1f);"     //title
                "#sqrt{N. integration pts};"                   //x-axis    
                "volume (mean & std.dev. of %i trials)",       //y-axis    
                dim, sphere_1_rad, sphere_2_rad, sphere_sep, n_evals_per_pt
            ));        

            graph_vals->Draw("SAME"); 


            //draw the graph of statistical errors
            canv->cd(i_canv + dims.size()); 
            gPad->SetLogx(1); 
            gPad->SetLeftMargin(0.15);
            gPad->SetRightMargin(0.0); 
            gPad->SetTopMargin(0.05);


            hist_frame = gPad->DrawFrame(
                sqrt(n_integ_pts_0 * 0.5), 
                0.,

                sqrt(n_integ_pts_0 * pow( 2, n_eval_levels ) * 2),
                max_stddev * (1. + y_buffer)
            );  

            hist_frame->SetTitle(Form(
                ";"                            //title
                "#sqrt{N. integration pts};"   //x-axis    
                "std.dev of %i trials",        //y-axis    
                n_evals_per_pt
            ));        

            graph_errors->Draw("SAME");

            i_canv++; 
        }

        canv->SaveAs(path_graphic); 
        return 0; 
    }   
    //______________________________________________________________________________________

    
    //______________________________________________________________________________________
    if (plots_to_make=="methods") {

        const vector<int> dims = {3, 5, 10}; 

        auto canv = new TCanvas("c", "canv", 1400, 800); 
        canv->Divide(dims.size(), 1); 

        //starting number of points to integrate with
        const unsigned long int n_integ_pts_0 = 75; 

        //number of times we re-evaluate the integral for each point. 
        // the 'statistical error' will be defined as the stddev of the results 
        // for each separate 'n_integ_pts' value
        const int n_evals_per_pt = 50; 

        //each time, we will multiply by 2. 
        // so the final number of points is = n_integ_pts * pow( 2, n_eval_levels-1 )
        int n_eval_levels = 18; 

        //size of the spheres, and the offset of their centers 
        const double sphere_1_rad = 1.0; 
        const double sphere_2_rad = 1.0; 
        const double sphere_sep   = 0.0; 

        int i_canv=1; 
        for (const int dim : dims) {
            
            double min_y{+1.e30}, max_y{-1.e30}; 
            double max_stddev{0.};

            printf("evaluating %id sphere...\n", dim); 

            struct GraphPoints_t { vector<double> pts, mean, stddev; };
            
            //_______________________________________________________________________________________________________________
            auto compute_mean_stddev = [&](double n_integrate_pts, GraphPoints_t& points, IntegratorType type)
            {
                vector<double> vals;

                for (int n=0; n<n_evals_per_pt; n++) {

                    double volume = compute_sphere_overlap(
                        dim, 
                        n_integrate_pts, 
                        sphere_1_rad, 
                        sphere_2_rad, 
                        sphere_sep, type
                    ); 

                    vals.push_back(volume); 
                }
                //now, compute the stddev and mean
                double mean, stddev; 
                compute_vector_stddev_mean(vals, mean, stddev);

                points.pts     .push_back(sqrt(n_integrate_pts));
                points.mean    .push_back(mean); 
                points.stddev  .push_back(stddev); 
                
                //find the min and max y-vals in the dataset 
                min_y = min<double>( mean - stddev, min_y ); 
                max_y = max<double>( mean + stddev, max_y ); 

                max_stddev = max<double>( stddev, max_stddev );
            }; 
            //_______________________________________________________________________________________________________________
            
            GraphPoints_t pts_pseudo, pts_quasi, pts_grid; 

            unsigned long int n_integ_pts = n_integ_pts_0; 
            
            for (int i=0; i<n_eval_levels; i++) {

                compute_mean_stddev(n_integ_pts, pts_pseudo, kMontecarlo);
                compute_mean_stddev(n_integ_pts, pts_quasi,  kQuasirandom);
                compute_mean_stddev(n_integ_pts, pts_grid,   kGrid);

                //double the number of stone-throwing tries
                n_integ_pts *= 2; 
            }
            
            //now we're done evaluating all points. 
            vector<double> x_errors(n_eval_levels, 0.); 

            //make the 3 graphs
            vector<TGraphErrors*> gvec; 
            for (auto pt : vector<GraphPoints_t*>{ &pts_pseudo, &pts_quasi, &pts_grid }) {
                gvec.push_back(
                    new TGraphErrors(
                        pt->pts.size(),

                        pt->pts.data(), 
                        pt->mean.data(), 

                        x_errors.data(),
                        pt->stddev.data()
                    )
                );
            }
            
            canv->cd(i_canv); 
            
            //how much to extend the y-axis by (in a fraction of the total amplitude) 
            const double y_buffer = 0.1; 

            auto hist_frame = gPad->DrawFrame(
                sqrt(n_integ_pts_0 * 0.5), 
                min_y - y_buffer*(max_y - min_y),

                sqrt(n_integ_pts_0 * pow( 2, n_eval_levels ) * 2),
                max_y + y_buffer*(max_y - min_y)
            );  

            hist_frame->SetTitle(Form(
                "unit %i-ball volume;"     //title
                "#sqrt{N. integration pts};"                   //x-axis    
                "volume (mean & std.dev. of %i trials)",       //y-axis    
                dim, n_evals_per_pt
            ));        
            
            //draw the graph of calculated volume values
            gPad->SetLogx(1); 
            gPad->SetLeftMargin(0.15);
            gPad->SetRightMargin(0.0); 
            
            //rename our 3 graphs for readability 
            auto& g_pseudo = gvec[0];  
            g_pseudo->SetMarkerStyle(kOpenCircle); 
            g_pseudo->SetMarkerColor(kRed); 
            g_pseudo->SetLineColor(kRed);
            g_pseudo->Draw("SAME");     

            auto& g_quasi  = gvec[1]; 
            g_quasi->SetMarkerStyle(kOpenTriangleUp); 
            g_quasi->SetMarkerColor(kBlack); 
            g_quasi->SetLineColor(kBlack);
            g_quasi->Draw("SAME"); 
           
            auto& g_grid   = gvec[2];    
            g_grid->SetMarkerStyle(kCross); 
            g_grid->SetMarkerColor(kBlue); 
            g_grid->SetLineColor(kBlue);
            g_grid->Draw("SAME"); 

            auto legend = new TLegend(0.6,0.1, 1.0,0.4); 
            legend->SetHeader("integrator");
            legend->AddEntry(g_pseudo,  "pseudo");
            legend->AddEntry(g_quasi,   "quasi");
            legend->AddEntry(g_grid,    "grid");
            legend->Draw(); 

            i_canv++; 
        }

        canv->SaveAs(path_graphic); 
        return 0; 
    }


    return 0; 
}