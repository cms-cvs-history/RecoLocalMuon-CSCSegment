/*
 *  Basic analyzer class which accesses CSCSegment and CSCRecHits
 *  
 *  Author: Shih-Chuan Kao  - UC Riverside
 *
 */

#include "CSCMTCCAnalysis.h"

#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCSegment.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2D.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCChamber.h"
#include "Geometry/CSCGeometry/interface/CSCLayer.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/GeometryVector//interface/GlobalPoint.h"
#include "DataFormats/GeometryVector//interface/GlobalVector.h"
#include "DataFormats/GeometryVector//interface/LocalPoint.h"
#include "DataFormats/GeometryVector//interface/LocalVector.h"
 
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

// Constructor
CSCMTCCAnalysis::CSCMTCCAnalysis(const edm::ParameterSet& pset){

  // Get the various input parameters
  debug               = pset.getUntrackedParameter<bool>("debug");
  rootFileName        = pset.getUntrackedParameter<std::string>("rootFileName");
  recHitLabel         = pset.getUntrackedParameter<std::string>("recHitLabel");
  cscSegmentLabel     = pset.getUntrackedParameter<std::string>("cscSegmentLabel");
  maxdxdz             = pset.getUntrackedParameter<double>("maxSlopedxdz");
  maxdydz             = pset.getUntrackedParameter<double>("maxSlopedydz");
  x_margin_L          = pset.getUntrackedParameter<int>("x_margin_L");
  x_margin_R          = pset.getUntrackedParameter<int>("x_margin_R");
  y_margin_T          = pset.getUntrackedParameter<double>("y_margin_T");
  y_margin_B          = pset.getUntrackedParameter<double>("y_margin_B");

  if(debug) std::cout << "[CSCMTCCAnalysis] Constructor called" << std::endl;
 
  FILE *mapfile;
  mapfile = fopen("CSCMTCC_MAP.txt","w+t");  
  
  // Create the root file
  theFile = new TFile(rootFileName.c_str(), "RECREATE");

  // Book the histograms
  //  hRHPMEa  = new H2DRecHit1("ME_0_1");
    hRHPMEb  = new H2DRecHit2("ME_0");

  // Begin loop histograms for all types of chambers
  char dets[6];
  int cb_i0=0;
  for (int st = 1; st < 5; st++){
      for (int rg = 1; rg < 5; rg++ ) {
          if ((rg==3 && st!=1) || (rg==2 && st==4))
          break;
          sprintf(dets,"ME_%d_%d",st,rg);
          hRHPME[cb_i0]  = new H2DRecHit(dets);
          std::cout <<"hDet_" <<cb_i0 << " = " << dets << std::endl;
          cb_i0++;
      }
  }

  // Begin loop histograms for each individual chamber and write this info into a mapfile
  char det_str[9];
  int cb_i1=0;
  int cb_i2=0;
  for (int st = 1; st < 5; st++){
      for (int rg = 1; rg < 4; rg++ ) {
          if (rg==1 && st!=1){
             for (int cb = 14; cb < 17; cb++){
                 sprintf(det_str,"ME_%d_%d_%d",st,rg,cb);
                 hChamber1[cb_i1]  = new H2DRecHit1(det_str);
                 std::cout <<"hchamber1_" <<cb_i1 << " = " << det_str << std::endl;
                 fprintf (mapfile, "1 %d %d %d %d\n",cb_i1,st,rg,cb);
                 cb_i1++;
             }
          }
          else {
             if ((rg==3 && st!=1) || (rg==2 && st==4))
             break;
             for (int cb = 27; cb < 33; cb++) {
                 sprintf(det_str,"ME_%d_%d_%d",st,rg,cb);
                 hChamber2[cb_i2]  = new H2DRecHit1(det_str);
                 std::cout <<"hchamber2_" <<cb_i2 << " = " << det_str << std::endl;
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
  
  if (debug) std::cout << "[CSCMTCCAnalysis] Destructor called" << std::endl;
  // Write the histos to file
  theFile->cd();
  //hRHPMEa ->Write();
  hRHPMEb ->Write(); 
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
  //delete hRHPMEa;
   delete hRHPMEb; 
  theFile->Close();
  if (debug) std::cout << "************* Finished writing histograms to file" << std::endl;
}

// The Analysis  (the main)

void CSCMTCCAnalysis::analyze(const edm::Event & event, const edm::EventSetup& eventSetup){
 
  if (event.id().event()%100 == 0) std::cout << " Event analysed #Run: " << event.id().run()
					<< " #Event: " << event.id().event() << std::endl;
  
  // Get the CSC Geometry :
  edm::ESHandle<CSCGeometry> cscGeom;
  eventSetup.get<MuonGeometryRecord>().get(cscGeom);
 
  // Get the Segments collection :
  edm::Handle<CSCSegmentCollection> cscSegments; 
  event.getByLabel(cscSegmentLabel, cscSegments);  
  if (debug) std::cout << "   #cscSegments: " << cscSegments->size() << std::endl;

  // Get the RecHits collection :
  edm::Handle<CSCRecHit2DCollection> recHits; 
  event.getByLabel(recHitLabel, recHits);  
  if (debug) std::cout << "   #RecHits: " << recHits->size() << std::endl;
  
  // Build Iterator for Calibrated Segments
  H2DRecHit *histo = 0;
  H2DRecHit1 *histo1 = 0;
  H2DRecHit2 *histo2 = 0;
  

  double xorgc = 999.0;
  double yorgc = 999.0;
  double err1[4] = {99.0};

  double SKrsduWth[6]={0.0};
  double SKrsdu[6]={0.0};

  double dx_a[6] ={0.0};
  double rht_y[6] ={0.0};
  int rht_ch[6] ={0};

  bool rh_type[6] ={false,false,false,false,false,false};
  bool rh_side = false;
  double X2c  = 999.0;
  int nhitsc = 999;
  nu_seg1 = 0; 
  seg_count1=false;
  bool no_gatti=false;
  bool y_edge=false;
  bool wrong_rh_err=false;

  for (int i=0; i<4; i++) {
      DetNu[i]=999;
  }

  for(CSCSegmentCollection::const_iterator segIt_1 = cscSegments->begin(); segIt_1 != cscSegments->end(); segIt_1++) 
  {
      int dof = (*segIt_1).degreesOfFreedom();
      double dofl = static_cast<double>(dof);
      CSCDetId idc = (CSCDetId)(*segIt_1).cscDetId();

      LocalVector v1 = (*segIt_1).localDirection();
      LocalPoint seg1_org = (*segIt_1).localPosition();

      nhitsc = (*segIt_1).nRecHits();
      X2c = (*segIt_1).chi2()/dofl;
      xorgc = seg1_org.x();
      yorgc = seg1_org.y();

      double slx1 = v1.x() / v1.z();
      double sly1 = v1.y() / v1.z();
      double RecX2[6]={0.0};
      int rh_info[6][5] ={{999},{999}};
      int ch_count =0;
      int CFEB_flag = 99;
      int CFEB_ck = 99;
      int CFEB_nu = 0;
      int nu_1ch_rh=0;

      //(1) Limit segments in fiducial volume
      rh_V0.clear();

      const std::vector<CSCRecHit2D>& rh_V2 = (*segIt_1).specificRecHits();
      for (std::vector<CSCRecHit2D>::const_iterator rh_i0 = rh_V2.begin(); rh_i0!=rh_V2.end(); ++rh_i0)
      { 

          rh_V0.push_back(*rh_i0);   

          CSCDetId idr1 = (CSCDetId)(*rh_i0).cscDetId();
          LocalPoint rh1_xyz = (*rh_i0).localPosition();
          const CSCLayer* csclayer = cscGeom->layer( idr1 );
          int nstrips =  (csclayer->geometry())->numberOfStrips(); 
          double y_bound =  ((csclayer->geometry())->length())/2.0; 

          ch_count =0;
          rh_info[idr1.layer()-1][0] = idr1.layer();
          // get the wire group of rechit
          int wire_nu = (csclayer->geometry())->nearestWire(rh1_xyz);
          rh_info[idr1.layer()-1][4] = (csclayer->geometry())->wireGroup(wire_nu);

          // Check the channel distribution of rechits
          const std::vector<int> &ch_nu = (*rh_i0).channels();
          for (std::vector<int>::const_iterator ch_it = ch_nu.begin(); ch_it!=ch_nu.end(); ++ch_it)
          {
              // check the fiducial volume
              if ((*ch_it >= nstrips-x_margin_R+1) || (*ch_it <= x_margin_L) ) {
                 no_gatti = true;
              }
              if (( rh1_xyz.y() > (y_bound - y_margin_T) )||( rh1_xyz.y() < (y_margin_B - y_bound) )) {
                 y_edge = true;
              }
              //<1> fill strip number(1~80) of the rechit for each layer
              ch_count += 1;
              rh_info[idr1.layer()-1][ch_count] = *ch_it;

              //<2> check the # of fired CFEB
              if ( CFEB_ck != ((*ch_it-1)/16) ) {
                 CFEB_nu += 1;
                 CFEB_ck = (*ch_it-1)/16;
              }
          }
          // pick up the segments which may be the CFEB effect victims -- w/ one "1-channel hit"
          if (ch_nu.size()==1)  {
             nu_1ch_rh +=1;
             CFEB_flag = ( nu_1ch_rh >=2 ? 98  : idr1.layer() );
          }
          // look at the CFEB effect victims -- 6 hits -> 5hits
          bool det_flag = true;
          if (( idc.station()==1 && idc.ring()==1 ) || 
              ( idc.station()==1 && idc.ring()==3 ) ||
              ( idc.station()==2 && idc.ring()==1 ) ||
              ( idc.station()==3 && idc.ring()==1 ) ){
              det_flag = false;
          }
          if (nhitsc==5 && det_flag) {
              histo2 = hRHPMEb;
              for (int k=1; k<4; k++){
                  histo2->Fill_A5(rh_info[idr1.layer()-1][0],rh_info[idr1.layer()-1][k],rh_info[idr1.layer()-1][4]);
              }
          }
          if (nhitsc==6 && det_flag) {
              histo2 = hRHPMEb;
              for (int k=1; k<4; k++){
                  histo2->Fill_A6(rh_info[idr1.layer()-1][0],rh_info[idr1.layer()-1][k],rh_info[idr1.layer()-1][4]);
              }
          }
      }
     
    
      if ( no_gatti || y_edge )
      break;
      

      // (1)Counting the segments/chamber in each event
      segment_chamber(idc);
      // (1) End   

      // (2) loop all hits in the segment - quick survey of the x pull
      CSCSegment fq_seg = *segIt_1;
      SKFitSlope(cscGeom, rh_V0.size(), rh_V0);
      x_pull(&fq_seg, cscGeom);
      for (int k=0; k<6; k++){
          dx_a[k] = pull[k];
      }
      // (2) End   


      // (3) look at the properties of 6 hits segments
      if (nhitsc ==6) {
         if (!wrong_rh_err ){ 
            err1[0] = sqrt(SKErr[0]);
            err1[1] = sqrt(SKErr[1]);
            err1[2] = sqrt(SKErr[2]);
            err1[3] = sqrt(SKErr[3]);
         }
   
         // ==1== x pull calculation  ====
         CSCSegment fq_seg = *segIt_1;
         SKFitSlope(cscGeom, rh_V0.size(), rh_V0);
         x_pull(&fq_seg, cscGeom);
         // ==2== residual funciton test ====
         // Residual < Rdphi / x / y / 5hitsfit >
         residual(&fq_seg, cscGeom);
         // ==3== classify the in-strip/inter-strip case ====
         // ===== and record the hit information         ====
         double XRec[6]={0.0};
         double XFit[6]={0.0};
         double rht_y[6]={0.0};
         for (int k=0; k<6; k++) {
             hit_info(fhit_cont[k],det_fit[k], cscGeom);
             rh_type[k] = rh_type_i;
             rh_side = rh_side_i;
             XRec[k]=(rhit_cont[k]).x()-strip_x;
             XFit[k]=(fhit_cont[k]).x()-strip_x;
             rht_y[k]=(rhit_cont[k]).y();
         }
         // ==4== rotation ====
         rotation(&fq_seg, fhit_cont ,cscGeom);
         for (int k=0; k<6; k++) {
             SKrsduWth[k]= (rh_u[k]-fit_u[k]) / cont[k];
         }
         // ======================================

    
         // Filling histograms crazily for 6 layers of  each individual chamber  ex: ME_2_2_27
         // Filling (a) x residual(cm) , (b) u(rotated x) residual(strip width) , 
         //         (1) Rdphi residual(strip width) for each layer , (2) Rdphi residual(cm)
         //         (3) x Pull(cm) , (4) y residual(cm)
         //         (5) the local y vs. the Rdphi residuals(in terms of cm and strip width)   
         // dmap[0] is the flag for different detector type numbering 
         // dmap[0]=1  => det# = 14 ~ 16  ; dmap[0]=2  => det# = 27 ~ 32

         if ( (fabs(slx1) < maxdxdz) && (fabs(sly1)< maxdydz) && (sly1<0.5) && (!wrong_rh_err) ) {
            int dmap[5]={99,99,99,99,99};
            ifstream dmapping("CSCMTCC_MAP.txt");
            while (!dmapping.eof()) {
                  dmapping >> dmap[0] >> dmap[1] >> dmap[2] >> dmap[3] >> dmap[4];
                  if (dmap[0]==1 && idc.station()==dmap[2] && idc.ring()==dmap[3] && idc.chamber()==dmap[4] ){
                   histo1 = hChamber1[dmap[1]];
                   if ( nu_1ch_rh == 0 ) {
                     // Filling (a) x residual(cm) , (b) u residual(strip width) 
                     histo1->Fill_f(resid_X[0],resid_X[1],resid_X[2],resid_X[3],resid_X[4],resid_X[5],SKrsduWth[0],SKrsduWth[1],SKrsduWth[2],SKrsduWth[3],SKrsduWth[4],SKrsduWth[5]);
                     // Filling (1) Rdphi residual(strip width), (2) Rdphi residual(cm)
                     histo1->Fill_1(resid_RdfWth[0],resid_RdfWth[1],resid_RdfWth[2],resid_RdfWth[3],resid_RdfWth[4],resid_RdfWth[5],resid_Rdf[0],resid_Rdf[1],resid_Rdf[2],resid_Rdf[3],resid_Rdf[4],resid_Rdf[5]);
                     // Filling (3) x Pull(cm)
                     histo1->Fill_2(pull[0],pull[1],pull[2],pull[3],pull[4],pull[5]);
                     // Filling (4) y residual(cm)
                     histo1->Fill_3(resid_Y[0],resid_Y[1],resid_Y[2],resid_Y[3],resid_Y[4],resid_Y[5]);
                     // (5) look at the relation between local y and Rdphi residual
                     // inter-strip case
                     if (rh_type[3]==false) {   
                         histo1->Fill_4_3(resid_Rdf[3],resid_RdfWth[3],rht_y[3]);
                         if (rh_side){
                             histo1->Fill_4_3P(resid_RdfWth[3]);
                         }
                         if (!rh_side) {
                             histo1->Fill_4_3N(resid_RdfWth[3]);
                         }
                     }
                     // in-strip case
                     if (rh_type[3]==true) {   
                         histo1->Fill_4a_3(resid_Rdf[3],resid_RdfWth[3],rht_y[3]);
                     }
                     histo1->Fill_4b(resid_Rdf[0],rht_y[0],resid_Rdf[1],rht_y[1],resid_Rdf[2],rht_y[2],resid_Rdf[3],rht_y[3],resid_Rdf[4],rht_y[4],resid_Rdf[5],rht_y[5]);
                     for (int k=0; k<6; k++ ) {
                         histo1->Fill_4c(resid_Rdf[k],resid_RdfWth[k],rht_y[k],XFit[k],XRec[k]);
                     }

                   }
                     // (6) the pull and residual with CFEB boundary effect
                    //if (CFEB_flag <=6 && !cross_seg1) {
                    if (CFEB_flag <=6 && (CFEB_nu==1) ) {
                       histo1->Fill_5(pull[CFEB_flag-1],resid_Rdf[CFEB_flag-1],CFEB_flag,xRecErr[CFEB_flag-1],slx1);
                       histo1->Fill_6(pull[6-CFEB_flag],resid_Rdf[6-CFEB_flag],CFEB_flag);
                    }
                  }
                  if (dmap[0]==2 && idc.station()==dmap[2] && idc.ring()==dmap[3] && idc.chamber()==dmap[4] ){
                   histo1 = hChamber2[dmap[1]];
                   if ( nu_1ch_rh == 0 ){
                     histo1->Fill_f(resid_X[0],resid_X[1],resid_X[2],resid_X[3],resid_X[4],resid_X[5],SKrsduWth[0],SKrsduWth[1],SKrsduWth[2],SKrsduWth[3],SKrsduWth[4],SKrsduWth[5]);
                     histo1->Fill_1(resid_RdfWth[0],resid_RdfWth[1],resid_RdfWth[2],resid_RdfWth[3],resid_RdfWth[4],resid_RdfWth[5],resid_Rdf[0],resid_Rdf[1],resid_Rdf[2],resid_Rdf[3],resid_Rdf[4],resid_Rdf[5]);
                     histo1->Fill_2(pull[0],pull[1],pull[2],pull[3],pull[4],pull[5]);
                     histo1->Fill_3(resid_Y[0],resid_Y[1],resid_Y[2],resid_Y[3],resid_Y[4],resid_Y[5]);
                     if (rh_type[3]==false) {   
                         histo1->Fill_4_3(resid_Rdf[3],resid_RdfWth[3],rht_y[3]);
                         if (rh_side){
                             histo1->Fill_4_3P(resid_RdfWth[3]);
                         }
                         if (!rh_side) {
                             histo1->Fill_4_3N(resid_RdfWth[3]);
                         }
                     }
                     if (rh_type[3]==true) {   
                         histo1->Fill_4a_3(resid_Rdf[3],resid_RdfWth[3],rht_y[3]);
                     }
                     histo1->Fill_4b(resid_Rdf[0],rht_y[0],resid_Rdf[1],rht_y[1],resid_Rdf[2],rht_y[2],resid_Rdf[3],rht_y[3],resid_Rdf[4],rht_y[4],resid_Rdf[5],rht_y[5]);
                     for (int k=0; k<6; k++ ) {
                         histo1->Fill_4c(resid_Rdf[k],resid_RdfWth[k],rht_y[k],XFit[k],XRec[k]);
                     }
                   }
                    if (CFEB_flag <=6 && (CFEB_nu==1) ) {
                       histo1->Fill_5(pull[CFEB_flag-1],resid_Rdf[CFEB_flag-1],CFEB_flag,xRecErr[CFEB_flag-1],slx1);
                       histo1->Fill_6(pull[6-CFEB_flag],resid_Rdf[6-CFEB_flag],CFEB_flag);
                    }
                  }
            }
         }

      }


     // Looping histograms for all types of chambers
     int cbt_i=0;
     for (int st = 1; st < 5; st++){
          for (int rg = 1; rg < 5; rg++ ) {
          if ((rg==3 && st!=1) || (rg==2 && st==4))
          break;
              if ( (idc.station() == st) && (idc.ring() == rg) ){
                 //Fill the basic information of segments
                 histo = hRHPME[cbt_i];
                 histo->Fill_a(xorgc,yorgc,X2c,nhitsc,slx1,sly1,nu_1ch_rh);
                 // Fill the statistics of segment#/chamber in each event 
                 if (seg_count1){ 
                    histo->Fill_b(nu_seg1);
                    nu_seg1=1;
                    seg_count1=false;
                 }
                 // Fill the error information
                 // err1[]    -> SK fitting error for 6 hits segments
                 // xRecErr[] -> local position error of rechit
                 // xFitErr[] -> local position error from 5 hits fitting
                 if ((nhitsc ==6) && (!no_gatti) && (!wrong_rh_err) && (!y_edge)) {
                    histo->Fill_c(err1[0],err1[1],err1[2],err1[3],X2c,xRecErr[0],xRecErr[2],xRecErr[4],xFitErr[0] ,xFitErr[2],xFitErr[4]);
                    // Fill the residual information with some parameters 
                    for (int k = 0; k < 6; k++) {
                        histo->Fill_d(resid_X[k],SKrsdu[k],resid_RdfWth[k],resid_Rdf[k],pull[k],X2c,slx1,sly1,xRecErr[k],dx_a[k],rht_y[k],rht_ch[k],wire_group[k],RecX2[k]);
                    }
                    // look at the Rdphi residual of strip centre or strip edge vs local y
                    if ( (fabs(slx1) < maxdxdz) && (fabs(sly1)< maxdydz) && (sly1<0.5)){    
                           // hits around strip centre
                           if ( (rh_type[3]==true)&&( nu_1ch_rh == 0) ) {   
                              histo->Fill_h(resid_Rdf[3],rht_y[3]);
                           }
                           // hits at strip edge 
                           if ( (rh_type[3]==false)&&( nu_1ch_rh == 0) ) {   
                              histo->Fill_k(resid_Rdf[3],rht_y[3]);
                           } 
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
// *******************************
// *****  Utility Functions  *****
// *******************************

// number of Segment / chamber
void CSCMTCCAnalysis::segment_chamber( CSCDetId det_id ) {
      
      if (nu_seg1 == 0){ 
         nu_seg1 = 1;
         DetNu[0] = det_id.endcap();
         DetNu[1] = det_id.station();
         DetNu[2] = det_id.ring();
         DetNu[3] = det_id.chamber();
      } 
      else if ( (det_id.endcap() == DetNu[0]) && (det_id.ring() == DetNu[2]) && (det_id.station() == DetNu[1]) && (det_id.chamber() == DetNu[3]) ) {
              nu_seg1 = nu_seg1 + 1;
      }
      else { 
         seg_count1 = true;       
         DetNu[0] = det_id.endcap();
         DetNu[1] = det_id.station();
         DetNu[2] = det_id.ring();
         DetNu[3] = det_id.chamber();
      }

}

// rotation x(local coordinate) -> u(strip coordinate)
void CSCMTCCAnalysis::rotation(const CSCSegment* seg, LocalPoint fit_p[6], edm::ESHandle<CSCGeometry> cscGeom){

     const std::vector<CSCRecHit2D>& rh = (seg->specificRecHits());
     for (std::vector<CSCRecHit2D>::const_iterator rh_it = rh.begin(); rh_it != rh.end(); ++rh_it) 
     {
         // get the rotation angle
         CSCDetId det_id = (CSCDetId)(*rh_it).cscDetId();
         int it = det_id.layer()-1; 
         LocalPoint rh_p = (*rh_it).localPosition();
         LocalError rh_err = (*rh_it).localPositionError(); 
         double ang = 0.5*atan( 2.*rh_err.xy()/(rh_err.xx()-rh_err.yy()) ); 

         // rotate dx to du; x residual in strip-coordinate => shoulb be equivalent to Rdphi residual
         rh_u[it] = rh_p.x()*cos(ang)+rh_p.y()*sin(ang);
         fit_u[it]= (fit_p[it]).x()*cos(ang)+(fit_p[it]).y()*sin(ang); 
     }
}

//hit information - in-strip / inter-strip
void CSCMTCCAnalysis::hit_info(LocalPoint rh_p, CSCDetId det_id, edm::ESHandle<CSCGeometry> cscGeom){

     const CSCLayer* csclayer_1 = cscGeom->layer( det_id );
     int   hit_ch = (csclayer_1->geometry())->nearestStrip( rh_p );
     strip_x = (csclayer_1->geometry())->xOfStrip(hit_ch,rh_p.y());
     double strip_Wth = (csclayer_1->geometry())->stripPitch(rh_p);
     double strip_Ang = (csclayer_1->geometry())->stripAngle(hit_ch);
     // hits belong to inter-strip or in-strip 
     rh_type_i = ( fabs(rh_p.x() - strip_x) <= fabs(0.25*strip_Wth/sin(strip_Ang)) ? true : false );
     rh_side_i = ( (rh_p.x() >  strip_x ) ? true : false );
}

// residual calculation
void CSCMTCCAnalysis::residual(const CSCSegment* seg, edm::ESHandle<CSCGeometry> cscGeom){

     int nu_hits = seg->nRecHits();
     double fit5hits_X[6]={999.0};
     double rec_local_X[6]={99.0};
     double fit5hits_Y[6]={999.0};
     double rec_local_Y[6]={99.0};
     double delta_z[6]={0.0};
     double global_R[6]={999.0};
     double rh_phi[6]={999.0};
     double stripWth[6];

     for (int it = 0; it < nu_hits; it++){
         rh_Vec.clear();
         resid_X[it]=999.0;
         resid_Y[it]=999.0;
         resid_Rdf[it]=999.0;
         resid_RdfWth[it]=999.0;
         const std::vector<CSCRecHit2D>& rh = (seg->specificRecHits());
         for (std::vector<CSCRecHit2D>::const_iterator rh_it = rh.begin(); rh_it != rh.end(); ++rh_it) 
         { 
             CSCDetId det_id = (CSCDetId)(*rh_it).cscDetId();
             if ( (det_id.layer()-1) != it ) {
                rh_Vec.push_back(*rh_it);
             }

             // get the local z position: using CSClayer transfer Local XY -> Global XYZ
             //                      then using CSCChamber transfer Global XYZ -> Local XYZ
             LocalPoint rh_xyz = (*rh_it).localPosition();
             const CSCLayer* csclayer = cscGeom->layer( det_id );
             const CSCChamber* cscchamber = cscGeom->chamber( det_id );
             GlobalPoint rh_Gxyz = csclayer->toGlobal(rh_xyz);
             LocalPoint rh_Lxyz = cscchamber->toLocal(rh_Gxyz);
             if ( (det_id.layer()-1) == it ) {
                rec_local_X[it]=rh_xyz.x();
                rec_local_Y[it]=rh_xyz.y();
                delta_z[it]=rh_Lxyz.z();

                global_R[it] = sqrt( (rh_Gxyz.x()*rh_Gxyz.x()) + (rh_Gxyz.y()*rh_Gxyz.y()) );
                rh_phi[it] = rh_Gxyz.phi();
                det_fit[it] = (CSCDetId)(*rh_it).cscDetId();

                stripWth[it] = (csclayer->geometry())->stripPitch(rh_xyz);
                cont[it]=stripWth[it];

                rhit_cont[it]=rh_xyz;
                xRecErr[it]=((*rh_it).localPositionError()).xx();

                int wire_nu = (csclayer->geometry())->nearestWire(rh_xyz);
                wire_group[it] = (csclayer->geometry())->wireGroup(wire_nu);
             }
             

         }
         SKFitSlope(cscGeom,rh_Vec.size(),rh_Vec);
         fit5hits_X[it] = (SKFit[2]*delta_z[it]) + SKFit[0];
         fit5hits_Y[it] = (SKFit[3]*delta_z[it]) + SKFit[1];
         LocalPoint fit5hits = LocalPoint(fit5hits_X[it], fit5hits_Y[it], 0.);
         const CSCLayer* csclayer_fit = cscGeom->layer( det_fit[it] );
         GlobalPoint gfit5hits = csclayer_fit->toGlobal(fit5hits);

         fhit_cont[it] = fit5hits;
         xFitErr[it] = SKErr[0] + (SKErr[2]*delta_z[it]*delta_z[it]) +(2.0*delta_z[it]*SKErr[4]);

         resid_X[it]= rec_local_X[it]-fit5hits_X[it]; 
         resid_Y[it]= rec_local_Y[it]-fit5hits_Y[it]; 
   
         resid_Rdf[it] = global_R[it]*sin( rh_phi[it] - gfit5hits.phi() );
         resid_RdfWth[it] = resid_Rdf[it]/stripWth[it];
     }
}


// xpull calculation
void CSCMTCCAnalysis::x_pull(const CSCSegment* seg, edm::ESHandle<CSCGeometry> cscGeom){

     LocalVector vec = seg->localDirection();
     LocalPoint seg_org = seg->localPosition();
     //LocalError o_err = seg->localPositionError();
     //LocalError v_err = seg->localDirectionError();

     double slx = vec.x() / vec.z();

     for (int k=0; k<6; k++){
         pull[k]=99.0;
     }

     const std::vector<CSCRecHit2D>& rh = (seg->specificRecHits());
     for (std::vector<CSCRecHit2D>::const_iterator rh_it = rh.begin(); rh_it != rh.end(); ++rh_it) { 

         CSCDetId det_id = (CSCDetId)(*rh_it).cscDetId();
         LocalPoint rh_xyz = (*rh_it).localPosition();
         LocalError rh_err = (*rh_it).localPositionError();
         int j = det_id.layer()-1;

         // get the local z position: using CSClayer transfer Local XY -> Global XYZ
         //                      then using CSCChamber transfer Global XYZ -> Local XYZ
         const CSCLayer* csclayer = cscGeom->layer( det_id );
         const CSCChamber* cscchamber = cscGeom->chamber( det_id );
         GlobalPoint rh_Gxyz = csclayer->toGlobal(rh_xyz);
         LocalPoint rh_Lxyz = cscchamber->toLocal(rh_Gxyz);

         // Fitting position x = az + b
         double fit_x = ( slx*rh_Lxyz.z() ) + seg_org.x();
         //double fit_err =  SKErr[0] + (rh_Lxyz.z()*rh_Lxyz.z()*SKErr[2]) + (2.0*rh_Lxyz.z()*SKErr[4]);
         double fit_err =  SKErr[0] + (rh_Lxyz.z()*rh_Lxyz.z()*SKErr[2]) ;
         //double fit_err =  o_err.xx() + (rh_Lxyz.z()*rh_Lxyz.z()*v_err.xx()) + (2.0*rh_Lxyz.z()*SKErr[4]);

         // x pull = dx / sqrt(rh_err.x() - fit_err.x() )
         if (rh_err.xx()-fit_err < 0.0) continue;
         pull[j] = (rh_Lxyz.x() - fit_x) / sqrt(rh_err.xx()-fit_err);

         /*std::cout <<"pull"<<j<<" = "<<pull[j] << std::endl;
         std::cout <<"sk0= "<<SKErr[0] <<" sk2= "<<SKErr[2]<<"  sk4= "<<SKErr[4]<<std::endl;
         std::cout <<"o_err= "<<o_err.xx() <<" v_err= "<<v_err.xx()<<std::endl;
         std::cout <<"              dx =         "<< rh_Lxyz.x() - fit_x << std::endl;
         std::cout <<"sqrt(rh_err.xx()-fit_err)= "<< sqrt(rh_err.xx()-fit_err) << std::endl;*/
     }    

}


// Segment fitting from SK Algorithm
void CSCMTCCAnalysis::SKFitSlope(edm::ESHandle<CSCGeometry> cscGeom, int rechit_size, std::vector<CSCRecHit2D> rh_V00){
 
      CLHEP::HepMatrix M1(4,4,0);
      CLHEP::HepVector B1(4,0);
      CLHEP::HepMatrix Dmatrix1(rechit_size*2, 4);
      AlgebraicSymMatrix Wmatrix1(rechit_size*2, 0);
      int row1 = 0;
      for (std::vector<CSCRecHit2D>::const_iterator rh_i0 = rh_V00.begin(); rh_i0!=rh_V00.end(); ++rh_i0)
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
      	  CLHEP::HepMatrix IC(2,2);
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
      CLHEP::HepVector p1 = solve(M1, B1);
      SKFit[0] = p1(1);   // origin of x
      SKFit[1] = p1(2);   // origin of y
      SKFit[2] = p1(3);   // slope of x
      SKFit[3] = p1(4);   // slope of y
      // Solve the fitting errors
      int ierr=0;
      Wmatrix1.invert(ierr);
      AlgebraicSymMatrix Ematrix1 = Wmatrix1.similarityT(Dmatrix1);
      Ematrix1.invert(ierr);
      AlgebraicSymMatrix SKfitErr1 = Ematrix1;
      SKErr[0] = SKfitErr1[0][0];  // error of origin; xx
      SKErr[1] = SKfitErr1[1][1];  // error of origin; yy 
      SKErr[2] = SKfitErr1[2][2];  // error of slope; xx
      SKErr[3] = SKfitErr1[3][3];  // error of slope; yy
      SKErr[4] = SKfitErr1[0][2];  // error of origin-slope; xx
      SKErr[5] = SKfitErr1[1][3];  // error of origin-slope; yy

}

DEFINE_FWK_MODULE(CSCMTCCAnalysis);
