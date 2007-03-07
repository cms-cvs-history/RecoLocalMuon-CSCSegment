/*
 *  Basic analyzer class which accesses CSCSegment and CSCRecHits
 *
 *
 *  Author: Shih-Chuan Kao  - UC Riverside
 *          Dominique Fortin
 */

#include "CSCMTCCAnalysis.h"

#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include <Geometry/CSCGeometry/interface/CSCChamber.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"
 
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "TFile.h"
#include "TVector3.h"

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <stdio.h>
#include <algorithm>

using namespace std;
using namespace edm;

// Constructor
CSCMTCCAnalysis::CSCMTCCAnalysis(const ParameterSet& pset){

  // Get the various input parameters
  debug               = pset.getUntrackedParameter<bool>("debug");
  rootFileName        = pset.getUntrackedParameter<string>("rootFileName");
  recHitLabel         = pset.getUntrackedParameter<string>("recHitLabel");
  cscSegmentLabel     = pset.getUntrackedParameter<string>("cscSegmentLabel");
  maxdxdz             = pset.getUntrackedParameter<double>("maxSlopedxdz");
  maxdydz             = pset.getUntrackedParameter<double>("maxSlopedydz");
  x_margin_L          = pset.getUntrackedParameter<int>("x_margin_L");
  x_margin_R          = pset.getUntrackedParameter<int>("x_margin_R");
  y_margin_T          = pset.getUntrackedParameter<double>("y_margin_T");
  y_margin_B          = pset.getUntrackedParameter<double>("y_margin_B");

  if(debug) cout << "[CSCMTCCAnalysis] Constructor called" << endl;
 
  FILE *mapfile;
  mapfile = fopen("CSCMTCC_MAP.txt","w+t");  
  
  // Create the root file
  theFile = new TFile(rootFileName.c_str(), "RECREATE");

  // Book the histograms
  hRHPMEa  = new H2DRecHit1("ME_0_1");
//  hRHPMEb  = new H2DRecHit2("ME_0_2");

  // Begin loop histograms for all types of chambers
  char dets[6];
  int cb_i0=0;
  for (int st = 1; st < 5; st++){
      for (int rg = 1; rg < 5; rg++ ) {
          if ((rg==3 && st!=1) || (rg==2 && st==4))
          break;
          sprintf(dets,"ME_%d_%d",st,rg);
          hRHPME[cb_i0]  = new H2DRecHit(dets);
          cout <<"hDet_" <<cb_i0 << " = " << dets << endl;
          cb_i0++;
      }
  }

  // Begin loop histograms for all chambers
  char det_str[9];
  int cb_i1=0;
  int cb_i2=0;
  for (int st = 1; st < 5; st++){
      for (int rg = 1; rg < 4; rg++ ) {
          if (rg==1 && st!=1){
             for (int cb = 14; cb < 17; cb++){
                 sprintf(det_str,"ME_%d_%d_%d",st,rg,cb);
                 hChamber1[cb_i1]  = new H2DRecHit3(det_str);
                 cout <<"hchamber1_" <<cb_i1 << " = " << det_str << endl;
                 fprintf (mapfile, "1 %d %d %d %d\n",cb_i1,st,rg,cb);
                 cb_i1++;
             }
          }
          else {
             if ((rg==3 && st!=1) || (rg==2 && st==4))
             break;
             for (int cb = 27; cb < 33; cb++) {
                 sprintf(det_str,"ME_%d_%d_%d",st,rg,cb);
                 hChamber2[cb_i2]  = new H2DRecHit3(det_str);
                 cout <<"hchamber2_" <<cb_i2 << " = " << det_str << endl;
                 fprintf (mapfile, "2 %d %d %d %d\n",cb_i2,st,rg,cb);
                 cb_i2++;
             }
          }
          
      }
  }
  fclose(mapfile);
  // End loop histograms for all chambers
}

// Destructor
CSCMTCCAnalysis::~CSCMTCCAnalysis(){
  
  if (debug) cout << "[CSCMTCCAnalysis] Destructor called" << endl;
  // Write the histos to file
  theFile->cd();
  hRHPMEa ->Write();
  //hRHPMEb ->Write(); 
  // Begin loop histograms for all chambers
  for (int j=0; j < 9; j++){
      hChamber1[j]->Write();
      hRHPME[j]->Write();
  }
  for (int k=0; k < 30; k++){
      hChamber2[k]->Write();
  }
  // End loop histograms for all chambers*/

  // Begin loop histograms for all chambers
  for (int j=0; j < 9; j++){
      delete hChamber1[j];
      delete hRHPME[j];
  }
  for (int k=0; k < 30; k++){
      delete hChamber2[k];
  }
  // End loop histograms for all chambers*/
  delete hRHPMEa;
 // delete hRHPMEb; 
  theFile->Close();
  if (debug) cout << "************* Finished writing histograms to file" << endl;
}

// The Analysis  (the main)
void CSCMTCCAnalysis::analyze(const Event & event, const EventSetup& eventSetup){
 
  if (event.id().event()%100 == 0) cout << " Event analysed #Run: " << event.id().run()
					<< " #Event: " << event.id().event() << endl;
  
  // Get the CSC Geometry :
  ESHandle<CSCGeometry> cscGeom;
  eventSetup.get<MuonGeometryRecord>().get(cscGeom);
 
  // Get the Segments collection :
  Handle<CSCSegmentCollection> cscSegments; 
  event.getByLabel(cscSegmentLabel, cscSegments);  
  if (debug) cout << "   #cscSegments: " << cscSegments->size() << endl;

  // Get the RecHits collection :
  Handle<CSCRecHit2DCollection> recHits; 
  event.getByLabel(recHitLabel, recHits);  
  if (debug) cout << "   #RecHits: " << recHits->size() << endl;
  
  // Build Iterator for Calibrated Segments
  H2DRecHit *histo = 0;
  H2DRecHit1 *histo1 = 0;
//  H2DRecHit2 *histo2 = 0;
  H2DRecHit3 *histo3 = 0;

  //int c1=0;
  float xorgc = 999.0;
  float yorgc = 999.0;
  double err1[4] = {99.0};
  double err2[5] = {99.0};
  double err4[5] = {99.0};
  float xFitErr[6]={0.0};
  float xRecErr[6]={0.0};
  float rht_y[6] ={0.0};
  float rsd_a[6] ={0.0};
  float dx_a[6] ={0.0};
  float xPull[6]={0.0};
  float SKrsduWth[6]={0.0};
  float SKrsdu[6]={0.0};
  int rht_ch[6] ={0};
  bool rh_type[6] ={false,false,false,false,false,false};
  double rsd_Rdphi[6]={0.0};
  double rsd_RdphiWth[6]={0.0};
  double X2c  = 999.0;
  int nhitsc = 999;
  int DetNu[4]={999}; 
  int nu_seg1=0;
  bool seg_count1=false;
  bool no_gatti=false;
  bool wrong_rh_err=false;
  bool y_edge=false;

  for(CSCSegmentCollection::const_iterator segIt_1 = cscSegments->begin(); segIt_1 != cscSegments->end(); segIt_1++) 
  {
      int dof = (*segIt_1).degreesOfFreedom();
      double a = 1.000;
      double dofl = dof*a;
      CSCDetId idc = (CSCDetId)(*segIt_1).cscDetId();

      LocalVector v1 = (*segIt_1).localDirection();
      LocalPoint seg1_org = (*segIt_1).localPosition();

      nhitsc = (*segIt_1).nRecHits();
      X2c = (*segIt_1).chi2()/dofl;
      xorgc = seg1_org.x();
      yorgc = seg1_org.y();

      float xv1 = v1.x();
      float yv1 = v1.y();
      float zv1 = v1.z();
      float slx1 = xv1/zv1;
      float sly1 = yv1/zv1;

      // Limit segments in fiducial volume
      rh_V0.clear();
      const std::vector<CSCRecHit2D>& rh_V2 = (*segIt_1).specificRecHits();
      for (std::vector<CSCRecHit2D>::const_iterator rh_i0 = rh_V2.begin(); rh_i0!=rh_V2.end(); ++rh_i0)
      { 
          CSCDetId idr1 = (CSCDetId)(*rh_i0).cscDetId();
          LocalPoint rh1_xyz = (*rh_i0).localPosition();
          const CSCLayer* csclayer = cscGeom->layer( idr1 );
          int nstrips =  (csclayer->geometry())->numberOfStrips(); 
          float y_bound =  ((csclayer->geometry())->length())/2.0; 

          const std::vector<int> &ch_nu = (*rh_i0).channels();
          for (std::vector<int>::const_iterator ch_it = ch_nu.begin(); ch_it!=ch_nu.end(); ++ch_it)
          {
              if ((*ch_it >= nstrips-x_margin_R+1) || (*ch_it <= x_margin_L) ) {
                 no_gatti = true;
              }
              if (( rh1_xyz.y() > (y_bound - y_margin_T) )||( rh1_xyz.y() < (y_margin_B - y_bound) )) {
                 y_edge = true;
              }
          }
          rh_V0.push_back(*rh_i0);   
      }
      if (no_gatti || y_edge) 
      break;
      SKFitSlope(cscGeom,rh_V0.size());
      //End of Limit segments in fiducial volume

      // **Segments Count
      if (nu_seg1 == 0){ 
         nu_seg1 = 1;
         DetNu[0] = idc.endcap();
         DetNu[1] = idc.station();
         DetNu[2] = idc.ring();
         DetNu[3] = idc.chamber();
      } 
      else if ( (idc.endcap() == DetNu[0]) && (idc.ring() == DetNu[2]) && (idc.station() == DetNu[1]) && (idc.chamber() == DetNu[3]) ) {
              nu_seg1 = nu_seg1 + 1;
      }
      else { 
         seg_count1 = true;       
         DetNu[0] = idc.endcap();
         DetNu[1] = idc.station();
         DetNu[2] = idc.ring();
         DetNu[3] = idc.chamber();
      }
      // **Segments loop end   


      // loop all hits in the segment - quick survey of the x pull
      const std::vector<CSCRecHit2D>& rh_V = (*segIt_1).specificRecHits();
      float dz=0.0; 
      float dy_rh=0.0; 
      float dx_rh=0.0; 
      float rh_xfit=0.0; 
      float rh_yfit=0.0; 
      float rsdx[6]={0.0};
      LocalPoint fit_lp;

	 for (std::vector<CSCRecHit2D>::const_iterator rh_i = rh_V.begin(); rh_i!=rh_V.end(); ++rh_i)
         {
             LocalPoint rh_xyz = (*rh_i).localPosition();
             LocalError rh_err = (*rh_i).localPositionError();
             CSCDetId idr = (CSCDetId)(*rh_i).cscDetId();

             const CSCLayer* csclayer = cscGeom->layer( idr );
             const CSCChamber* cscchamber = cscGeom->chamber(idr);
             GlobalPoint grh_xyz = csclayer->toGlobal(rh_xyz);
             LocalPoint lrh_xyz = cscchamber->toLocal(grh_xyz);
 
             dz = lrh_xyz.z() - seg1_org.z();

             rh_xfit = slx1*dz + seg1_org.x();
             rh_yfit = sly1*dz + seg1_org.y();
             fit_lp = LocalPoint(rh_xfit, rh_yfit, 0.);
  
             dx_rh = rh_xyz.x() - rh_xfit;  
             dy_rh = rh_xyz.y() - rh_yfit;  

             wrong_rh_err = ( rh_err.xx() > 5.0 ? true : false);

             if (nhitsc == 6.0) {
                int it = idr.layer() - 1;
                float rh_fit_err =  SKErr[0] + (dz*dz*SKErr[2]) + (2.0*dz*SKErr[4]);
                dx_a[it] = dx_rh; 
                rsdx[it] = dx_rh / sqrt(rh_err.xx() - rh_fit_err);
             }

         }
         // End of Segment hits loop

      // check the err distribution
      if (nhitsc ==6){  
         if (!wrong_rh_err ){ 
            err1[0] = sqrt(SKErr[0]);
            err1[1] = sqrt(SKErr[1]);
            err1[2] = sqrt(SKErr[2]);
            err1[3] = sqrt(SKErr[3]);
            err2[0] = SKErr[0];
            err2[1] = SKErr[1];
            err2[2] = SKErr[2];
            err2[3] = SKErr[3];
            err2[4] = SKErr[4];
            histo1 = hRHPMEa;
            histo1->Fill_b(rsdx[0],rsdx[1],rsdx[2],rsdx[3],rsdx[4],rsdx[5]);
         }
      }
      // End of segment hits loop


      if (nhitsc ==6) {
         float SKrsdx[6]={0.0};
         float SKrsdy[6]={0.0};
         float SKfit_x[6]={0.0};
         float SKfit_y[6]={0.0};
         float rht_x[6]={0.0};
         float rht_phi[6] ={0.0};
         float stripWth[6] ={0.0};
         float gR = 0.0;
         float SKfit_u[6]={0.0};
         float rht_u[6]={0.0};
         float ang[6]={0.0};

         const std::vector<CSCRecHit2D>& rh_V1 = (*segIt_1).specificRecHits();
         CSCDetId idr1;
         CSCDetId idr1_a[6];
         GlobalPoint grh1_xyz;
         GlobalPoint grh1_xyz_a;
         GlobalPoint gfit_xyz;
         LocalPoint rh1_xyz;
         LocalPoint fit_xyz;
         LocalError rh1_err;
         float dzz[6]={0.0};

         for (int it2 = 0; it2 < 6; it2++){
            rh_V0.clear();
	    for (std::vector<CSCRecHit2D>::const_iterator rh_i2 = rh_V1.begin(); rh_i2!=rh_V1.end(); ++rh_i2)
            { 
                idr1 = (CSCDetId)(*rh_i2).cscDetId();
                rh1_xyz = (*rh_i2).localPosition();
                rh1_err = (*rh_i2).localPositionError();

                const CSCLayer* csclayer = cscGeom->layer( idr1 );
                const CSCChamber* cscchamber = cscGeom->chamber( idr1 );
                grh1_xyz = csclayer->toGlobal(rh1_xyz);
                LocalPoint lrh1_xyz = cscchamber->toLocal(grh1_xyz);



                // x Pull calculation for each layer!
                if ((idr1.layer()-1) == it2) {
                   idr1_a[it2] = (CSCDetId)(*rh_i2).cscDetId();
                   const CSCLayer* csclayer_a = cscGeom->layer( idr1_a[it2] );
                   grh1_xyz_a = csclayer_a->toGlobal(rh1_xyz);

                   rht_phi[it2] = grh1_xyz_a.phi();
                   rht_x[it2] = rh1_xyz.x();
                   rht_y[it2] = rh1_xyz.y();
                   dzz[it2] = lrh1_xyz.z();
                   stripWth[it2] = (csclayer->geometry())->stripPitch(rh1_xyz);

                   float dxx = rht_x[it2]- ( slx1*dzz[it2] + seg1_org.x() );
                   float rh1_fit_err =  err2[0] + (dzz[it2]*dzz[it2]*err2[2]) + (2.0*dzz[it2]*err2[4]);
                   xRecErr[it2] = rh1_err.xx();
                   xPull[it2] = dxx /sqrt(rh1_err.xx() - rh1_fit_err);  
                   // rotate the x to u
                   ang[it2] = 0.5*atan( 2.*rh1_err.xy()/(rh1_err.xx()-rh1_err.yy()) ); 
                   rht_u[it2] = rht_x[it2]*cos(ang[it2])+rht_y[it2]*sin(ang[it2]);
                   rht_ch[it2] = (csclayer->geometry())->nearestStrip(rh1_xyz);
                }

                // pick up the rest 5 hits and fill them in new vector rh_V0
                if ( (idr1.layer()-1) != it2 ) {
                   rh_V0.push_back(*rh_i2);
                }

            }

            // Fit the rest 5 hits to get un-bias residual
            SKFitSlope(cscGeom,rh_V0.size());
            err4[0] = SKErr[0]; // err^2 of origin x 
            err4[1] = SKErr[1]; // err^2 of origin y
            err4[2] = SKErr[2]; // err^2 of dx/dz 
            err4[3] = SKErr[3]; // err^2 of dy/dz 
            err4[4] = SKErr[4]; // err^2 of x*dx/dz 
	    double sk_x  = SKFit[0];
            double sk_y  = SKFit[1];
	    double sk_mxz = SKFit[2];
	    double sk_myz = SKFit[3];

            SKfit_x[it2]= sk_mxz*dzz[it2] + sk_x; 
            SKfit_y[it2]= sk_myz*dzz[it2] + sk_y;
 
            SKrsdx[it2]= rht_x[it2] - SKfit_x[it2];
            //SKrsdxWth[it2]= (rht_x[it2] - SKfit_x[it2])/stripWth[it2];
            SKrsdy[it2]= rht_y[it2] - SKfit_y[it2];
            rsd_a[it2] = SKrsdx[it2];
             
            // rotate dx to du
            SKfit_u[it2]=   ( sk_mxz*cos(ang[it2])+sk_myz*sin(ang[it2]) )*dzz[it2] 
                          + ( sk_x*cos(ang[it2])+sk_y*sin(ang[it2]) );
            SKrsdu[it2]= rht_u[it2] - SKfit_u[it2];
            SKrsduWth[it2]= SKrsdu[it2]/stripWth[it2];
            //////////// 

            xFitErr[it2] =  err4[0] + (err4[2]*dzz[it2]*dzz[it2]) +(2.0*dzz[it2]*err4[4]);

            fit_xyz = LocalPoint(SKfit_x[it2], SKfit_y[it2], 0.);  
            const CSCLayer* csclayer_a = cscGeom->layer( idr1_a[it2] );
            gfit_xyz = csclayer_a->toGlobal(fit_xyz);

            gR = sqrt( (gfit_xyz.x()*gfit_xyz.x()) + (gfit_xyz.y()*gfit_xyz.y()) );
            rsd_Rdphi[it2] = ( rht_phi[it2] - gfit_xyz.phi() )*gR;
            rsd_RdphiWth[it2] = rsd_Rdphi[it2]/stripWth[it2];

            // hit in centre of strip or edge of strip
            int fh_ch = (csclayer_a->geometry())->nearestStrip(fit_xyz);
            float angStrip = (csclayer_a->geometry())->stripAngle(fh_ch);
            float xshift =  SKfit_y[it2] / fabs(tan(angStrip));
            float x0OfStrip = (csclayer_a->geometry())->xOfStrip(fh_ch,fit_xyz.y());
            float x1OfStrip = x0OfStrip + xshift ;
            float fstripWth = (csclayer_a->geometry())->stripPitch(fit_xyz);
            if ( fabs(SKfit_x[it2] - x0OfStrip) >= (0.50*fstripWth) ){
               cout << "strip width= "<< fstripWth <<"  dx= " <<fabs(SKfit_x[it2] - x0OfStrip) <<endl;
               cout << "x_strip@0= "<<x0OfStrip <<"  x shift= "<<SKfit_y[it2]/tan(angStrip)<<endl;
               cout << "y = "<< SKfit_y[it2] <<" ang= "<<angStrip<<" tan(angle) = "<< tan(angStrip) <<endl;
               cout <<"===================================================="<<endl;
            }
             rh_type[it2]=( fabs(SKfit_x[it2] - x0OfStrip) <= (0.35*fstripWth) ? true : false );
        

         }

         //!! filling histograms for 6 layers of all chambers 
         if ( (fabs(slx1) < maxdxdz) && (fabs(sly1)< maxdydz) && (!no_gatti)&& (!wrong_rh_err) && (!y_edge)) {
 //           histo2 = hRHPMEb;
            int dmap[5]={99,99,99,99,99};
            ifstream dmapping("CSCMTCC_MAP.txt");
            while (!dmapping.eof()) {
                  dmapping >> dmap[0] >> dmap[1] >> dmap[2] >> dmap[3] >> dmap[4];
                  if (dmap[0]==1 && idc.station()==dmap[2] && idc.ring()==dmap[3] && idc.chamber()==dmap[4] ){
                     histo3 = hChamber1[dmap[1]];
                     histo3->Fill_f(SKrsdx[0],SKrsdx[1],SKrsdx[2],SKrsdx[3],SKrsdx[4],SKrsdx[5],SKrsduWth[0],SKrsduWth[1],SKrsduWth[2],SKrsduWth[3],SKrsduWth[4],SKrsduWth[5],xPull[0],xPull[1],xPull[2],xPull[3],xPull[4],xPull[5]);
                     histo3->Fill_i(SKrsdy[0],SKrsdy[1],SKrsdy[2],SKrsdy[3],SKrsdy[4],SKrsdy[5],rsd_Rdphi[0],rsd_Rdphi[1],rsd_Rdphi[2],rsd_Rdphi[3],rsd_Rdphi[4],rsd_Rdphi[5]);
                     //histo3->Fill_j(SKrsdu[0],SKrsdu[1],SKrsdu[2],SKrsdu[3],SKrsdu[4],SKrsdu[5]);
                     histo3->Fill_j(rsd_RdphiWth[0],rsd_RdphiWth[1],rsd_RdphiWth[2],rsd_RdphiWth[3],rsd_RdphiWth[4],rsd_RdphiWth[5]);
                     for (int k=0; k<6; k++ ) {
                         histo3->Fill_l(rsd_Rdphi[k],rsd_RdphiWth[k],rht_y[k]);
                     }
                  }
                  if (dmap[0]==2 && idc.station()==dmap[2] && idc.ring()==dmap[3] && idc.chamber()==dmap[4] ){
                     histo3 = hChamber2[dmap[1]];
                     histo3->Fill_f(SKrsdx[0],SKrsdx[1],SKrsdx[2],SKrsdx[3],SKrsdx[4],SKrsdx[5],SKrsduWth[0],SKrsduWth[1],SKrsduWth[2],SKrsduWth[3],SKrsduWth[4],SKrsduWth[5],xPull[0],xPull[1],xPull[2],xPull[3],xPull[4],xPull[5]);
                     histo3->Fill_i(SKrsdy[0],SKrsdy[1],SKrsdy[2],SKrsdy[3],SKrsdy[4],SKrsdy[5],rsd_Rdphi[0],rsd_Rdphi[1],rsd_Rdphi[2],rsd_Rdphi[3],rsd_Rdphi[4],rsd_Rdphi[5]);
                     //histo3->Fill_j(SKrsdu[0],SKrsdu[1],SKrsdu[2],SKrsdu[3],SKrsdu[4],SKrsdu[5]);
                     histo3->Fill_j(rsd_RdphiWth[0],rsd_RdphiWth[1],rsd_RdphiWth[2],rsd_RdphiWth[3],rsd_RdphiWth[4],rsd_RdphiWth[5]);
                     for (int k=0; k<6; k++ ) {
                         histo3->Fill_l(rsd_Rdphi[k],rsd_RdphiWth[k],rht_y[k]);
                     }
                  }
            }
         }
         //!! end of histograms filling

      }


     // Begin loop histograms for all types of chambers
     int cbt_i=0;
     for (int st = 1; st < 5; st++){
          for (int rg = 1; rg < 5; rg++ ) {
          if ((rg==3 && st!=1) || (rg==2 && st==4))
          break;
              if ( (idc.station() == st) && (idc.ring() == rg) ){
                 histo = hRHPME[cbt_i];
                 histo->Fill_c(xorgc,yorgc,X2c,nhitsc,slx1,sly1);
                 if (seg_count1){ 
                    histo->Fill_a(nu_seg1);
                    nu_seg1=1;
                    seg_count1=false;
                 }
                 if ((nhitsc ==6) && (!no_gatti) && (!wrong_rh_err) && (!y_edge)) {
                    histo->Fill_g(err1[0],err1[1],err1[2],err1[3],X2c,xRecErr[0],xRecErr[2],xRecErr[4],xFitErr[0] ,xFitErr[2],xFitErr[4]);
                    for (int k = 0; k < 6; k++) {
                        histo->Fill_e(rsd_a[k],SKrsdu[k],SKrsduWth[k],rsd_Rdphi[k],xPull[k],X2c,slx1,sly1,xRecErr[k],dx_a[k],rht_y[k],rht_ch[k]);
                        //if (k==2){
                           if (rh_type[k]==true){
                              histo->Fill_h(SKrsduWth[k],rht_y[k]);
                           } 
                           if (rh_type[k]==false){
                              histo->Fill_k(SKrsduWth[k],rht_y[k]);
                           } 
                        //}
                    }
                 } 
              }
              cbt_i++;
          }
     }
      // End loop histograms for all types of chambers
  }
  // End of Segment loop
}

// Segment fitting from SK Algorithm
void CSCMTCCAnalysis::SKFitSlope(ESHandle<CSCGeometry> cscGeom, int rechit_size){
 
      HepMatrix M1(4,4,0);
      HepVector B1(4,0);
      HepMatrix Dmatrix1(rechit_size*2, 4);
      AlgebraicSymMatrix Wmatrix1(rechit_size*2, 0);
      int row1 = 0;
      for (std::vector<CSCRecHit2D>::const_iterator rh_i0 = rh_V0.begin(); rh_i0!=rh_V0.end(); ++rh_i0)
      { 
          CSCDetId idr0 = (CSCDetId)(*rh_i0).cscDetId();
          LocalPoint rh0_xyz = (*rh_i0).localPosition();

          const CSCLayer* csclayer = cscGeom->layer( idr0 );
          const CSCChamber* cscchamber = cscGeom->chamber( idr0 );
          GlobalPoint grh0_xyz = csclayer->toGlobal(rh0_xyz);
          LocalPoint lrh0_xyz = cscchamber->toLocal(grh0_xyz);

          // SK Fit !!!
          // ptc: Local position of hit w.r.t. chamber
	  double u = rh0_xyz.x();
    	  double v = rh0_xyz.y();
       	  double z = lrh0_xyz.z();
          // ptc: Covariance matrix of local errors 
      	  HepMatrix IC(2,2);
	  IC(1,1) = rh_i0->localPositionError().xx();
          IC(1,2) = rh_i0->localPositionError().xy();
          IC(2,1) = IC(1,2); // since Cov is symmetric
   	  IC(2,2) = rh_i0->localPositionError().yy();
          //  ptc: Invert covariance matrix (and trap if it fails!)
	  int ierr = 0;
	  IC.invert(ierr); // inverts in place
    		// ptc: Note that IC is no longer Cov but Cov^-1
  	          M1(1,1) += IC(1,1);
	    	  M1(1,2) += IC(1,2);
	    	  M1(1,3) += IC(1,1) * z;
	     	  M1(1,4) += IC(1,2) * z;
	     	  B1(1) += u * IC(1,1) + v * IC(1,2);
	     	  M1(2,1) += IC(2,1);
	     	  M1(2,2) += IC(2,2);
	       	  M1(2,3) += IC(2,1) * z;
	     	  M1(2,4) += IC(2,2) * z;
	     	  B1(2) += u * IC(2,1) + v * IC(2,2);
	     	  M1(3,1) += IC(1,1) * z;
	     	  M1(3,2) += IC(1,2) * z;
	     	  M1(3,3) += IC(1,1) * z * z;
	     	  M1(3,4) += IC(1,2) * z * z;
	     	  B1(3) += ( u * IC(1,1) + v * IC(1,2) ) * z;
	    	  M1(4,1) += IC(2,1) * z;
	     	  M1(4,2) += IC(2,2) * z;
	     	  M1(4,3) += IC(2,1) * z * z;
	     	  M1(4,4) += IC(2,2) * z * z;
		  B1(4) += ( u * IC(2,1) + v * IC(2,2) ) * z;
	  // SK Fit End !!!!
               // SK Fit error calculation !
                  ++row1;
                  Dmatrix1(row1, 1) = 1.;
                  Dmatrix1(row1, 3) = z;
                  Wmatrix1(row1, row1)   = rh_i0->localPositionError().xx();
                  Wmatrix1(row1, row1+1) = rh_i0->localPositionError().xy();
                  ++row1;
                  Dmatrix1(row1, 2) = 1.;
                  Dmatrix1(row1, 4) = z;
                  Wmatrix1(row1, row1-1) = rh_i0->localPositionError().xy();
                  Wmatrix1(row1, row1)   = rh_i0->localPositionError().yy();
               // SK Fit error calculation end !
      }
      // Solve the matrix equation using CLHEP's 'solve'
      HepVector p1 = solve(M1, B1);
      SKFit[0] = p1(1);   // org_x
      SKFit[1] = p1(2);   // org_y
      SKFit[2] = p1(3);   // slope x
      SKFit[3] = p1(4);   // slope y
      // Solve the fitting errors
      int ierr=0;
      Wmatrix1.invert(ierr);
      AlgebraicSymMatrix Ematrix1 = Wmatrix1.similarityT(Dmatrix1);
      Ematrix1.invert(ierr);
      AlgebraicSymMatrix SKfitErr1 = Ematrix1;
      SKErr[0] = SKfitErr1[0][0];  // org_err.xx
      SKErr[1] = SKfitErr1[1][1];  // org_err.yy 
      SKErr[2] = SKfitErr1[2][2];  // slp_err.xx
      SKErr[3] = SKfitErr1[3][3];  // slp_err.yy
      SKErr[4] = SKfitErr1[0][2];  // org_slp_err.xx
      SKErr[5] = SKfitErr1[1][3];  // org_slp_err.yy

}

DEFINE_FWK_MODULE(CSCMTCCAnalysis)
