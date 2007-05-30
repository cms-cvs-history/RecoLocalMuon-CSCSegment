#ifndef CSCSegment_CSCMTCCHistograms_H
#define CSCSegment_CSCMTCCHistograms_H

/** \class CSCMTCCHistograms
 *  Collection of histograms for 1D DT RecHit test.
 *
 * Author: D. Fortin  - UC Riverside
 */

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TString.h"
#include <string>
#include <iostream>

using namespace std;

class H2DRecHit {
public:
  /// Constructor from collection name
  H2DRecHit(std::string name_) {
    TString N = name_.c_str();
    name=N;
//  Local X and Y position    
    hOrgX    = new TH1F(N+"_hOrgX", " segment origin X", 320, -80., 80.);
    hOrgY    = new TH1F(N+"_hOrgY", " segment origin Y", 400, -100., 100.);
//  Chi2
    hX2      = new TH1F(N+"_hX2", " segment Chi Square", 160, 0., 80.);
//  number of hits/segment 
    hNHits   = new TH1F(N+"_hNHits", " segment Hits number", 10, 0., 10.);
    hNSeg1   = new TH1F(N+"_hNSeg1", "# of segment/events ", 15, 0., 15.);
//  slope dx/dz and dy/dz
    hslx1      = new TH1F(N+"_hslx1", "slope from vector dx/dz ", 501, -3., 3.);
    hsly1      = new TH1F(N+"_hsly1", "slope from vector dy/dz ", 501, -3., 3.);
//  # of 1-strip rechit
    h1ch_rh  = new TH2F(N+"_h1ch_rh", "# of 1strip rechits vs # of rechits", 7, 0., 7., 7, 0., 7.);

//  residual for hits around strip centre
    hRdfRsd1_Y = new TH2F(N+"_hRdfRsd1_Y", "Rdphi residual(centre) vs Y", 20, -160., 160., 240, -0.6, 0.6);
//  residual for hits around strip edge
    hRdfRsd2_Y = new TH2F(N+"_hRdfRsd2_Y", "Rdphi residual(edge) vs Y", 20, -160., 160., 240, -0.6, 0.6);
//
    hRdfRsd_Y_a    = new TH2F(N+"_hRdfRsd_Y_a", "Rdphi residual(cm) vs Y; for the same type", 20, -160., 160., 240, -0.6, 0.6);
    hRdfRsdWth_Y_a = new TH2F(N+"_hRdfRsdWth_Y_a", "Rdphi residual(strip width) vs Y; for the same type", 20, -160., 160., 240, -0.6, 0.6);
    hRdfRsdWth_W_a = new TH2F(N+"_hRdfRsdWth_W_a", "Rdphi residual(strip width) vs Wiregroup; for the same type", 114, 0., 114., 240, -0.6, 0.6);
//  For residual study
    hRdphi_x   = new TH2F(N+"_hRdphi_x", "unbias Rdphi residual vs xRsd", 240, -0.8, 0.8, 240, -0.8, 0.8);
    hRdphi_u   = new TH2F(N+"_hRdphi_u", "unbias Rdphi residual vs uRsd", 240, -0.8, 0.8, 240, -0.8, 0.8);
    hxRsd_ch   = new TH2F(N+"_hxRsd_ch", "unbias x residual vs ch#", 100, 0., 100., 240, -0.6, 0.6);
    huRsd_ch   = new TH2F(N+"_huRsd_ch", "unbias u residual vs ch#", 100, 0., 100., 240, -0.6, 0.6);
    hxPull_Y   = new TH2F(N+"_hxPull_Y", " x Pull vs Y", 20, -160., 160., 105, -10., 10.);
    hRdfRsd_X2   = new TH2F(N+"_hRdfRsd_X2", " Rdphi Rsd vs X2", 160, 0., 80., 105, -1., 1.);
    hRdfRsd_slx  = new TH2F(N+"_hRdfRsd_slx", " Rdphi Rsd vs slop x", 40, -1., 1., 105, -1., 1.);
    hRdfRsd_sly  = new TH2F(N+"_hRdfRsd_sly", " Rdphi Rsd vs slop y", 120, -3., 3.,105, -1., 1.);
    hRdfRsd_err  = new TH2F(N+"_hRdfRsd_err", " Rdphi Rsd vs err x", 300, 0., 0.03,105, -1., 1.);
    hRdfRsd_dx   = new TH2F(N+"_hRdfRsd_dx", " Rdphi Rsd vs dx ", 240, -0.6, 0.6,105, -1., 1.);
    hRdfRsd_RecX2= new TH2F(N+"_hRdfRsd_RecX2", " Rdphi Rsd vs Rec X2", 160, 0., 80., 105, -1., 1.);

//  Errors
    hErr1OrgX  = new TH1F(N+"_hErr1OrgX", " Error1 origin X", 200, 0., 0.1);  
    hErr1OrgY  = new TH1F(N+"_hErr1OrgY", " Error1 origin Y", 125, 0., 1.0);  
    hErr1VecX  = new TH1F(N+"_hErr1VecX", " Error1 vector X", 200, 0., 0.04);  
    hErr1VecY  = new TH1F(N+"_hErr1VecY", " Error1 vector Y", 125, 0., 0.25);  
    hX2c6hits  = new TH1F(N+"_hX2c6hits", " 6 hits segment Chi Square", 250, 0., 100.);
  
    hxRecErr0  = new TH1F(N+"_hxRecErr0", " Error0 rec X", 300, 0., 0.015);  
    hxRecErr2  = new TH1F(N+"_hxRecErr2", " Error2 rec X", 300, 0., 0.015);  
    hxRecErr4  = new TH1F(N+"_hxRecErr4", " Error4 rec X", 300, 0., 0.015);  
    hxFitErr0  = new TH1F(N+"_hxFitErr0", " Error0 fit X", 300, 0., 0.015);  
    hxFitErr2  = new TH1F(N+"_hxFitErr2", " Error2 fit X", 300, 0., 0.015);  
    hxFitErr4  = new TH1F(N+"_hxFitErr4", " Error4 fit X", 300, 0., 0.015);  

  }


  /// Constructor from collection name and TFile.
  H2DRecHit(TString name_, TFile* file) {
    name=name_;
// X
    hOrgX     = (TH1F *) file->Get(name+"_hOrgX");
// Y
    hOrgY     = (TH1F *) file->Get(name+"_hOrgY");
// Chi2
    hX2       = (TH1F *) file->Get(name+"_hX2");
// number of hits
    hNHits    = (TH1F *) file->Get(name+"_hNHits");
    hNSeg1    = (TH1F *) file->Get(name+"_hNSeg1");
//
    hslx1       = (TH1F *) file->Get(name+"_hslx1");
    hsly1       = (TH1F *) file->Get(name+"_hsly1");
//
    h1ch_rh  = (TH2F *) file->Get(name+"_h1ch_rh");
//
    hRdfRsd1_Y  = (TH2F *) file->Get(name+"_hRdfRsd1_Y");
    hRdfRsd2_Y  = (TH2F *) file->Get(name+"_hRdfRsd2_Y");
//
    hRdfRsd_Y_a     = (TH2F *) file->Get(name+"_hRdfRsd_Y_a");
    hRdfRsdWth_Y_a  = (TH2F *) file->Get(name+"_hRdfRsdWth_Y_1");
    hRdfRsdWth_W_a  = (TH2F *) file->Get(name+"_hRdfRsdWth_W_1");
//
    hRdphi_x    = (TH2F *) file->Get(name+"_hRdphi_x");
    hRdphi_u    = (TH2F *) file->Get(name+"_hRdphi_u");
    hxRsd_ch    = (TH2F *) file->Get(name+"_hxRsd_ch");
    huRsd_ch    = (TH2F *) file->Get(name+"_huRsd_ch");
    hxPull_Y    = (TH2F *) file->Get(name+"_hxPull_Y");
    hRdfRsd_X2    = (TH2F *) file->Get(name+"_hRdfRsd_X2");
    hRdfRsd_slx   = (TH2F *) file->Get(name+"_hRdfRsd_slx");
    hRdfRsd_sly   = (TH2F *) file->Get(name+"_hRdfRsd_sly");
    hRdfRsd_err   = (TH2F *) file->Get(name+"_hRdfRsd_err");
    hRdfRsd_dx    = (TH2F *) file->Get(name+"_hRdfRsd_dx");
    hRdfRsd_RecX2 = (TH2F *) file->Get(name+"_hRdfRsd_RecX2");
//
    hErr1OrgX  = (TH1F *) file->Get(name+"_hErr1OrgX");
    hErr1OrgY  = (TH1F *) file->Get(name+"_hErr1OrgY");
    hErr1VecX  = (TH1F *) file->Get(name+"_hErr1VecX");
    hErr1VecY  = (TH1F *) file->Get(name+"_hErr1VecY");
    hX2c6hits  = (TH1F *) file->Get(name+"_hX2c6hits");

    hxRecErr0  = (TH1F *) file->Get(name+"_hxRecErr0");
    hxRecErr2  = (TH1F *) file->Get(name+"_hxRecErr2");
    hxRecErr4  = (TH1F *) file->Get(name+"_hxRecErr4");
    hxFitErr0  = (TH1F *) file->Get(name+"_hxFitErr0");
    hxFitErr2  = (TH1F *) file->Get(name+"_hxFitErr2");
    hxFitErr4  = (TH1F *) file->Get(name+"_hxFitErr4");
  }



  /// Destructor
  virtual ~H2DRecHit() {
// X and Y
    delete hOrgX;
    delete hOrgY;
// Chi2
    delete hX2;
// number of hits
    delete hNHits;
    delete hNSeg1;
// slope
    delete hslx1;
    delete hsly1;
//
    delete h1ch_rh;
//
    delete hRdfRsd1_Y;
    delete hRdfRsd2_Y;
// 
    delete hRdfRsd_Y_a;
    delete hRdfRsdWth_Y_a;
    delete hRdfRsdWth_W_a;
//
    delete hRdphi_x;
    delete hRdphi_u;
    delete hxRsd_ch;
    delete huRsd_ch;
    delete hxPull_Y;
    delete hRdfRsd_X2;
    delete hRdfRsd_slx;
    delete hRdfRsd_sly;
    delete hRdfRsd_err;
    delete hRdfRsd_dx;
    delete hRdfRsd_RecX2;
//
    delete hErr1OrgX;
    delete hErr1OrgY;
    delete hErr1VecX;
    delete hErr1VecY;
    delete hX2c6hits;

    delete hxRecErr0;
    delete hxRecErr2;
    delete hxRecErr4;
    delete hxFitErr0;
    delete hxFitErr2;
    delete hxFitErr4;

  }


  /// Fill all the histos
  void Fill_a(float xorgc, float yorgc, double X2c, int nhitsc, float slx1, float sly1, int nu_1ch_rh) {
// X
    hOrgX->Fill(xorgc);
// Y  
    hOrgY->Fill(yorgc);
// Chi2
    hX2->Fill(X2c);
// # of hits
    hNHits->Fill(nhitsc);
// slope of dx/dz and dy/dz
    hslx1->Fill(slx1);
    hsly1->Fill(sly1);
// # of 1-strip rechit
    h1ch_rh->Fill(nu_1ch_rh,nhitsc);
  
  }

// Fill the segment and nhits info
  void Fill_b(int nu_seg1) {
       hNSeg1->Fill(nu_seg1);
  }

  void Fill_c(float Err1OrgX, float Err1OrgY, float Err1VecX, float Err1VecY, double X2c, float xRecErr0,float xRecErr2,float xRecErr4, float xFitErr0, float xFitErr2, float xFitErr4 ) {

       hErr1OrgX->Fill(Err1OrgX);
       hErr1OrgY->Fill(Err1OrgY);
       hErr1VecX->Fill(Err1VecX);
       hErr1VecY->Fill(Err1VecY);
       hX2c6hits->Fill(X2c); 

       hxRecErr0->Fill(xRecErr0);
       hxRecErr2->Fill(xRecErr2);
       hxRecErr4->Fill(xRecErr4);
       hxFitErr0->Fill(xFitErr0);
       hxFitErr2->Fill(xFitErr2);
       hxFitErr4->Fill(xFitErr4);
  }

  void Fill_d(float xRsd, float uRsd, float rsd_RdphiWth, float rsd_Rdphi, float xPull, double X2, float slx1, float sly1,float xerr, float dx, float rht_y, float rht_ch, float wire_group, float RecX2) {

       hRdfRsd_Y_a->Fill(rht_y,rsd_Rdphi);
       hRdfRsdWth_Y_a->Fill(rht_y,rsd_RdphiWth);
       hRdfRsdWth_W_a->Fill(wire_group,rsd_RdphiWth);

       hRdphi_x->Fill(xRsd,rsd_Rdphi);
       hRdphi_u->Fill(uRsd,rsd_Rdphi);

       huRsd_ch->Fill(rht_ch,uRsd);
       hxRsd_ch->Fill(rht_ch,xRsd);

       hxPull_Y->Fill(rht_y,xPull);

       hRdfRsd_X2->Fill(X2,rsd_Rdphi);
       hRdfRsd_slx->Fill(slx1,rsd_Rdphi);
       hRdfRsd_sly->Fill(sly1,rsd_Rdphi);
       hRdfRsd_err->Fill(xerr,rsd_Rdphi);
       hRdfRsd_dx->Fill(dx,rsd_Rdphi);
       hRdfRsd_RecX2->Fill(RecX2,rsd_Rdphi);
  }

// residual for hits around strips centre
  void Fill_h(float rsd_RdphiWth, float rht_y) {
       hRdfRsd1_Y->Fill(rht_y,rsd_RdphiWth);
  }
// residual for hits at strips edge
  void Fill_k(float rsd_RdphiWth, float rht_y) {
       hRdfRsd2_Y->Fill(rht_y,rsd_RdphiWth);
  }


  /// Write all the histos to currently opened file
  void Write() {
// X
    hOrgX->Write();
// Y
    hOrgY->Write();
// Chi2
    hX2->Write();
// # of hits
    hNHits->Write();
    hNSeg1->Write();
//
    hslx1->Write();
    hsly1->Write();
//
    h1ch_rh->Write();
//
    hRdfRsd1_Y->Write();
    hRdfRsd2_Y->Write();
//
    hRdfRsd_Y_a->Write();
    hRdfRsdWth_Y_a->Write();
    hRdfRsdWth_W_a->Write();
    hRdphi_x->Write();
    hRdphi_u->Write();
    hxRsd_ch->Write();
    huRsd_ch->Write();
    hxPull_Y->Write();
    hRdfRsd_X2->Write();
    hRdfRsd_slx->Write();
    hRdfRsd_sly->Write();
    hRdfRsd_err->Write();
    hRdfRsd_dx->Write();
    hRdfRsd_RecX2->Write();
//
    hErr1OrgX->Write();
    hErr1OrgY->Write();
    hErr1VecX->Write();
    hErr1VecY->Write();
    hX2c6hits->Write();

    hxRecErr0->Write();
    hxRecErr2->Write();
    hxRecErr4->Write();
    hxFitErr0->Write();
    hxFitErr2->Write();
    hxFitErr4->Write();
  }

// X
    TH1F *hOrgX;
// Y
    TH1F *hOrgY;
// Chi2
    TH1F *hX2;
// # of hits
    TH1F *hNHits;
    TH1F *hNSeg1;
//
    TH1F *hslx1;
    TH1F *hsly1;
//
    TH2F *h1ch_rh;
//
    TH2F *hRdfRsd1_Y;
    TH2F *hRdfRsd2_Y;
//
    TH2F *hRdfRsd_Y_a;
    TH2F *hRdfRsdWth_Y_a;
    TH2F *hRdfRsdWth_W_a;
//
    TH2F *hRdphi_x;
    TH2F *hRdphi_u;
    TH2F *hxRsd_ch;
    TH2F *huRsd_ch;
    TH2F *hxPull_Y;
    TH2F *hRdfRsd_X2;
    TH2F *hRdfRsd_slx;
    TH2F *hRdfRsd_sly;
    TH2F *hRdfRsd_err;
    TH2F *hRdfRsd_dx;
    TH2F *hRdfRsd_RecX2;
//
    TH1F *hErr1OrgX;
    TH1F *hErr1OrgY;
    TH1F *hErr1VecX;
    TH1F *hErr1VecY;
    TH1F *hX2c6hits;

    TH1F *hxRecErr0;
    TH1F *hxRecErr2;
    TH1F *hxRecErr4;
    TH1F *hxFitErr0;
    TH1F *hxFitErr2;
    TH1F *hxFitErr4;

  TString name;
};

class H2DRecHit1 {
public:

  H2DRecHit1(std::string name_) {
    TString N3 = name_.c_str();
    name=N3;

    hSKrsdx1 = new TH1F(N3+"_hSKrsdx1", " x residual in 1st layer", 101, -1.01, 1.01);
    hSKrsdx2 = new TH1F(N3+"_hSKrsdx2", " x residual in 2st layer", 101, -1.01, 1.01);
    hSKrsdx3 = new TH1F(N3+"_hSKrsdx3", " x residual in 3st layer", 101, -1.01, 1.01);
    hSKrsdx4 = new TH1F(N3+"_hSKrsdx4", " x residual in 4st layer", 101, -1.01, 1.01);
    hSKrsdx5 = new TH1F(N3+"_hSKrsdx5", " x residual in 5st layer", 101, -1.01, 1.01);
    hSKrsdx6 = new TH1F(N3+"_hSKrsdx6", " x residual in 6st layer", 101, -1.01, 1.01);

    hSKrsdxWth1 = new TH1F(N3+"_hSKrsdxWth1", " x residual(strip width unit) in 1st layer", 101, -1.01, 1.01);
    hSKrsdxWth2 = new TH1F(N3+"_hSKrsdxWth2", " x residual(strip width unit) in 2st layer", 101, -1.01, 1.01);
    hSKrsdxWth3 = new TH1F(N3+"_hSKrsdxWth3", " x residual(strip width unit) in 3st layer", 101, -1.01, 1.01);
    hSKrsdxWth4 = new TH1F(N3+"_hSKrsdxWth4", " x residual(strip width unit) in 4st layer", 101, -1.01, 1.01);
    hSKrsdxWth5 = new TH1F(N3+"_hSKrsdxWth5", " x residual(strip width unit) in 5st layer", 101, -1.01, 1.01);
    hSKrsdxWth6 = new TH1F(N3+"_hSKrsdxWth6", " x residual(strip width unit) in 6st layer", 101, -1.01, 1.01);

    hxPull1 = new TH1F(N3+"_hxPull1", " x Pull in 1st layer", 101, -10.1, 10.1);
    hxPull2 = new TH1F(N3+"_hxPull2", " x Pull in 2st layer", 101, -10.1, 10.1);
    hxPull3 = new TH1F(N3+"_hxPull3", " x Pull in 3st layer", 101, -10.1, 10.1);
    hxPull4 = new TH1F(N3+"_hxPull4", " x Pull in 4st layer", 101, -10.1, 10.1);
    hxPull5 = new TH1F(N3+"_hxPull5", " x Pull in 5st layer", 101, -10.1, 10.1);
    hxPull6 = new TH1F(N3+"_hxPull6", " x Pull in 6st layer", 101, -10.1, 10.1);

    hSKrsdy1 = new TH1F(N3+"_hSKrsdy1", " y residual in 1st layer", 101, -8.01, 8.01);
    hSKrsdy2 = new TH1F(N3+"_hSKrsdy2", " y residual in 2st layer", 101, -8.01, 8.01);
    hSKrsdy3 = new TH1F(N3+"_hSKrsdy3", " y residual in 3st layer", 101, -8.01, 8.01);
    hSKrsdy4 = new TH1F(N3+"_hSKrsdy4", " y residual in 4st layer", 101, -8.01, 8.01);
    hSKrsdy5 = new TH1F(N3+"_hSKrsdy5", " y residual in 5st layer", 101, -8.01, 8.01);
    hSKrsdy6 = new TH1F(N3+"_hSKrsdy6", " y residual in 6st layer", 101, -8.01, 8.01);

    hrsd_Rdphi1 = new TH1F(N3+"_hrsd_Rdphi1", " Rdphi residual in 1st layer", 101, -0.3, 0.3);
    hrsd_Rdphi2 = new TH1F(N3+"_hrsd_Rdphi2", " Rdphi residual in 2st layer", 101, -0.3, 0.3);
    hrsd_Rdphi3 = new TH1F(N3+"_hrsd_Rdphi3", " Rdphi residual in 3st layer", 101, -0.3, 0.3);
    hrsd_Rdphi4 = new TH1F(N3+"_hrsd_Rdphi4", " Rdphi residual in 4st layer", 101, -0.3, 0.3);
    hrsd_Rdphi5 = new TH1F(N3+"_hrsd_Rdphi5", " Rdphi residual in 5st layer", 101, -0.3, 0.3);
    hrsd_Rdphi6 = new TH1F(N3+"_hrsd_Rdphi6", " Rdphi residual in 6st layer", 101, -0.3, 0.3);

    hrsd_RdphiWth1 = new TH1F(N3+"_hrsd_RdphiWth1", " Rdphi residual in 1st layer", 101, -0.3, 0.3);
    hrsd_RdphiWth2 = new TH1F(N3+"_hrsd_RdphiWth2", " Rdphi residual in 2st layer", 101, -0.3, 0.3);
    hrsd_RdphiWth3 = new TH1F(N3+"_hrsd_RdphiWth3", " Rdphi residual in 3st layer", 101, -0.3, 0.3);
    hrsd_RdphiWth4 = new TH1F(N3+"_hrsd_RdphiWth4", " Rdphi residual in 4st layer", 101, -0.3, 0.3);
    hrsd_RdphiWth5 = new TH1F(N3+"_hrsd_RdphiWth5", " Rdphi residual in 5st layer", 101, -0.3, 0.3);
    hrsd_RdphiWth6 = new TH1F(N3+"_hrsd_RdphiWth6", " Rdphi residual in 6st layer", 101, -0.3, 0.3);

    hrsd_RdphiWthP = new TH1F(N3+"_hrsd_RdphiWthP", " Rdphi residual @ +side ", 101, -0.3, 0.3);
    hrsd_RdphiWthN = new TH1F(N3+"_hrsd_RdphiWthN", " Rdphi residual @ -side ", 101, -0.3, 0.3);

    hRdfRsd_Y1_3= new TH2F(N3+"_hRdfRsd_Y1_3", "inter-strips Rdphi residual vs Y", 20, -160., 160., 101, -0.3, 0.3);
    hRdfRsd_Y2_3= new TH2F(N3+"_hRdfRsd_Y2_3", "in-Strip Rdphi residual vs Y", 20, -160., 160., 101, -0.3, 0.3);
    hRdfRsdWth_Y1_3= new TH2F(N3+"_hRdfRsdWth_Y1_3", "inter-strips Rdphi residual(Strip Width) vs Y", 20, -160., 160., 241, -0.6, 0.6);
    hRdfRsdWth_Y2_3= new TH2F(N3+"_hRdfRsdWth_Y2_3", "in-strip Rdphi residual(Strip Width) vs Y", 20, -160., 160., 241, -0.6, 0.6);
    
    hRdf_Y_1= new TH2F(N3+"_hRdf_Y_1", "Rdphi residual vs Y @ Layer1", 20, -160., 160., 101, -0.3, 0.3);
    hRdf_Y_2= new TH2F(N3+"_hRdf_Y_2", "Rdphi residual vs Y @ Layer2", 20, -160., 160., 101, -0.3, 0.3);
    hRdf_Y_3= new TH2F(N3+"_hRdf_Y_3", "Rdphi residual vs Y @ Layer3", 20, -160., 160., 101, -0.3, 0.3);
    hRdf_Y_4= new TH2F(N3+"_hRdf_Y_4", "Rdphi residual vs Y @ Layer4", 20, -160., 160., 101, -0.3, 0.3);
    hRdf_Y_5= new TH2F(N3+"_hRdf_Y_5", "Rdphi residual vs Y @ Layer5", 20, -160., 160., 101, -0.3, 0.3);
    hRdf_Y_6= new TH2F(N3+"_hRdf_Y_6", "Rdphi residual vs Y @ Layer6", 20, -160., 160., 101, -0.3, 0.3);
    hRdf_Y_0= new TH2F(N3+"_hRdf_Y_0", "Rdphi residual vs Y for all ", 20, -160., 160., 101, -0.3, 0.3);

    hRdfWth_Y_0= new TH2F(N3+"_hRdfWth_Y_0", "Rdphi residual vs Y(strip Width) for all ", 20, -160., 160., 241, -0.6, 0.6);
    hXRec_XFit= new TH2F(N3+"_hXRec_XFit", " X_Rec vs. X_Fit ", 200, -1., 1., 200, -1., 1.);
    
    hxPull_CFEB0 = new TH2F(N3+"_hxPull_CFEB0", " x Pulls around CFEB edge", 7,0.,7., 101, -10.1, 10.1);
    hrsd_Rdphi_CFEB0 = new TH2F(N3+"_hrsd_Rdphi_CFEB0", "Rdphi residual around CFEB edge", 7,0.,7., 101, -1.0, 1.0);
    hxPull_CFEB1 = new TH2F(N3+"_hxPull_CFEB1", " x Pull away form CFEB edge", 7,0.,7., 101, -10.1, 10.1);
    hrsd_Rdphi_CFEB1 = new TH2F(N3+"_hrsd_Rdphi_CFEB1", "Rdphi residual away from CFEB edge", 7,0.,7., 101, -1.0, 1.0);
    hErr_CFEB  = new TH2F(N3+"_hErr_CFEB", " xRecHit Error ", 7,0.,7., 100, 0., 0.1);
    hSlope_CFEB  = new TH2F(N3+"_hSlope_CFEB0", " slope dx/dz ", 7,0.,7., 100, -1., 1.);

  }

  H2DRecHit1(TString name_, TFile* file) {
    name=name_;

    hSKrsdx1    = (TH1F *) file->Get(name+"_hSKrsdx1");
    hSKrsdx2    = (TH1F *) file->Get(name+"_hSKrsdx2");
    hSKrsdx3    = (TH1F *) file->Get(name+"_hSKrsdx3");
    hSKrsdx4    = (TH1F *) file->Get(name+"_hSKrsdx4");
    hSKrsdx5    = (TH1F *) file->Get(name+"_hSKrsdx5");
    hSKrsdx6    = (TH1F *) file->Get(name+"_hSKrsdx6");

    hSKrsdxWth1    = (TH1F *) file->Get(name+"_hSKrsdxWth1");
    hSKrsdxWth2    = (TH1F *) file->Get(name+"_hSKrsdxWth2");
    hSKrsdxWth3    = (TH1F *) file->Get(name+"_hSKrsdxWth3");
    hSKrsdxWth4    = (TH1F *) file->Get(name+"_hSKrsdxWth4");
    hSKrsdxWth5    = (TH1F *) file->Get(name+"_hSKrsdxWth5");
    hSKrsdxWth6    = (TH1F *) file->Get(name+"_hSKrsdxWth6");

    hxPull1    = (TH1F *) file->Get(name+"_hxPull1");
    hxPull2    = (TH1F *) file->Get(name+"_hxPull2");
    hxPull3    = (TH1F *) file->Get(name+"_hxPull3");
    hxPull4    = (TH1F *) file->Get(name+"_hxPull4");
    hxPull5    = (TH1F *) file->Get(name+"_hxPull5");
    hxPull6    = (TH1F *) file->Get(name+"_hxPull6");

    hSKrsdy1    = (TH1F *) file->Get(name+"_hSKrsdy1");
    hSKrsdy2    = (TH1F *) file->Get(name+"_hSKrsdy2");
    hSKrsdy3    = (TH1F *) file->Get(name+"_hSKrsdy3");
    hSKrsdy4    = (TH1F *) file->Get(name+"_hSKrsdy4");
    hSKrsdy5    = (TH1F *) file->Get(name+"_hSKrsdy5");
    hSKrsdy6    = (TH1F *) file->Get(name+"_hSKrsdy6");

    hrsd_Rdphi1    = (TH1F *) file->Get(name+"_hrsd_Rdphi1");
    hrsd_Rdphi2    = (TH1F *) file->Get(name+"_hrsd_Rdphi2");
    hrsd_Rdphi3    = (TH1F *) file->Get(name+"_hrsd_Rdphi3");
    hrsd_Rdphi4    = (TH1F *) file->Get(name+"_hrsd_Rdphi4");
    hrsd_Rdphi5    = (TH1F *) file->Get(name+"_hrsd_Rdphi5");
    hrsd_Rdphi6    = (TH1F *) file->Get(name+"_hrsd_Rdphi6");

    hrsd_RdphiWth1    = (TH1F *) file->Get(name+"_hrsd_RdphiWth1");
    hrsd_RdphiWth2    = (TH1F *) file->Get(name+"_hrsd_RdphiWth2");
    hrsd_RdphiWth3    = (TH1F *) file->Get(name+"_hrsd_RdphiWth3");
    hrsd_RdphiWth4    = (TH1F *) file->Get(name+"_hrsd_RdphiWth4");
    hrsd_RdphiWth5    = (TH1F *) file->Get(name+"_hrsd_RdphiWth5");
    hrsd_RdphiWth6    = (TH1F *) file->Get(name+"_hrsd_RdphiWth6");

    hrsd_RdphiWthP    = (TH1F *) file->Get(name+"_hrsd_RdphiWthP");
    hrsd_RdphiWthN    = (TH1F *) file->Get(name+"_hrsd_RdphiWthN");

    hRdfRsd_Y1_3 = (TH2F *) file->Get(name+"_hRdfRsd_Y1_3");
    hRdfRsd_Y2_3 = (TH2F *) file->Get(name+"_hRdfRsd_Y2_3");
    hRdfRsdWth_Y1_3 = (TH2F *) file->Get(name+"_hRdfRsdWth_Y1_3");
    hRdfRsdWth_Y2_3 = (TH2F *) file->Get(name+"_hRdfRsdWth_Y2_3");

    hRdf_Y_1 = (TH2F *) file->Get(name+"_hRdf_Y_1");
    hRdf_Y_2 = (TH2F *) file->Get(name+"_hRdf_Y_2");
    hRdf_Y_3 = (TH2F *) file->Get(name+"_hRdf_Y_3");
    hRdf_Y_4 = (TH2F *) file->Get(name+"_hRdf_Y_4");
    hRdf_Y_5 = (TH2F *) file->Get(name+"_hRdf_Y_5");
    hRdf_Y_6 = (TH2F *) file->Get(name+"_hRdf_Y_6");
    hRdf_Y_0 = (TH2F *) file->Get(name+"_hRdf_Y_0");
    hRdfWth_Y_0 = (TH2F *) file->Get(name+"_hRdfWth_Y_0");
    
    hXRec_XFit = (TH2F *) file->Get(name+"_hXRec_XFit");
    
    hxPull_CFEB0        = (TH2F *) file->Get(name+"_hxPull_CFEB0");
    hrsd_Rdphi_CFEB0    = (TH2F *) file->Get(name+"_hrsd_Rdphi_CFEB0");
    hxPull_CFEB1        = (TH2F *) file->Get(name+"_hxPull_CFEB1");
    hrsd_Rdphi_CFEB1    = (TH2F *) file->Get(name+"_hrsd_Rdphi_CFEB1");
    hErr_CFEB           = (TH2F *) file->Get(name+"_hErr_CFEB");
    hSlope_CFEB         = (TH2F *) file->Get(name+"_hSlope_CFEB");

  } 

  /// Destructor
  virtual ~H2DRecHit1() {

    delete hSKrsdx1;
    delete hSKrsdx2;
    delete hSKrsdx3;
    delete hSKrsdx4;
    delete hSKrsdx5;
    delete hSKrsdx6;

    delete hSKrsdxWth1;
    delete hSKrsdxWth2;
    delete hSKrsdxWth3;
    delete hSKrsdxWth4;
    delete hSKrsdxWth5;
    delete hSKrsdxWth6;

    delete hxPull1;
    delete hxPull2;
    delete hxPull3;
    delete hxPull4;
    delete hxPull5;
    delete hxPull6;

    delete hSKrsdy1;
    delete hSKrsdy2;
    delete hSKrsdy3;
    delete hSKrsdy4;
    delete hSKrsdy5;
    delete hSKrsdy6;

    delete hrsd_Rdphi1;
    delete hrsd_Rdphi2;
    delete hrsd_Rdphi3;
    delete hrsd_Rdphi4;
    delete hrsd_Rdphi5;
    delete hrsd_Rdphi6;

    delete hrsd_RdphiWth1;
    delete hrsd_RdphiWth2;
    delete hrsd_RdphiWth3;
    delete hrsd_RdphiWth4;
    delete hrsd_RdphiWth5;
    delete hrsd_RdphiWth6;

    delete hrsd_RdphiWthP;
    delete hrsd_RdphiWthN;

    delete hRdfRsd_Y1_3;
    delete hRdfRsd_Y2_3;
    delete hRdfRsdWth_Y1_3;
    delete hRdfRsdWth_Y2_3;

    delete hRdf_Y_1;
    delete hRdf_Y_2;
    delete hRdf_Y_3;
    delete hRdf_Y_4;
    delete hRdf_Y_5;
    delete hRdf_Y_6;
    delete hRdf_Y_0;
    delete hRdfWth_Y_0;
    
    delete hXRec_XFit;

    delete hxPull_CFEB0;
    delete hrsd_Rdphi_CFEB0;
    delete hxPull_CFEB1;
    delete hrsd_Rdphi_CFEB1;
    delete hErr_CFEB;
    delete hSlope_CFEB;

  }

  void Fill_f(float SKrsdx1, float SKrsdx2,float SKrsdx3,float SKrsdx4,float SKrsdx5,float SKrsdx6,float SKrsdxWth1, float SKrsdxWth2,float SKrsdxWth3,float SKrsdxWth4,float SKrsdxWth5,float SKrsdxWth6) {

       hSKrsdx1->Fill(SKrsdx1);
       hSKrsdx2->Fill(SKrsdx2);
       hSKrsdx3->Fill(SKrsdx3);
       hSKrsdx4->Fill(SKrsdx4);
       hSKrsdx5->Fill(SKrsdx5);
       hSKrsdx6->Fill(SKrsdx6);

       hSKrsdxWth1->Fill(SKrsdxWth1);
       hSKrsdxWth2->Fill(SKrsdxWth2);
       hSKrsdxWth3->Fill(SKrsdxWth3);
       hSKrsdxWth4->Fill(SKrsdxWth4);
       hSKrsdxWth5->Fill(SKrsdxWth5);
       hSKrsdxWth6->Fill(SKrsdxWth6);
  }

  void Fill_1(float rsd_RdphiWth1, float rsd_RdphiWth2,float rsd_RdphiWth3,float rsd_RdphiWth4,float rsd_RdphiWth5,float rsd_RdphiWth6,float rsd_Rdphi1, float rsd_Rdphi2,float rsd_Rdphi3,float rsd_Rdphi4,float rsd_Rdphi5,float rsd_Rdphi6) {

       hrsd_RdphiWth1->Fill(rsd_RdphiWth1);
       hrsd_RdphiWth2->Fill(rsd_RdphiWth2);
       hrsd_RdphiWth3->Fill(rsd_RdphiWth3);
       hrsd_RdphiWth4->Fill(rsd_RdphiWth4);
       hrsd_RdphiWth5->Fill(rsd_RdphiWth5);
       hrsd_RdphiWth6->Fill(rsd_RdphiWth6);

       hrsd_Rdphi1->Fill(rsd_Rdphi1);
       hrsd_Rdphi2->Fill(rsd_Rdphi2);
       hrsd_Rdphi3->Fill(rsd_Rdphi3);
       hrsd_Rdphi4->Fill(rsd_Rdphi4);
       hrsd_Rdphi5->Fill(rsd_Rdphi5);
       hrsd_Rdphi6->Fill(rsd_Rdphi6);

  }
 
  void Fill_2(float xPull1, float xPull2,float xPull3,float xPull4,float xPull5,float xPull6){

       hxPull1->Fill(xPull1);
       hxPull2->Fill(xPull2);
       hxPull3->Fill(xPull3);
       hxPull4->Fill(xPull4);
       hxPull5->Fill(xPull5);
       hxPull6->Fill(xPull6);

  }

  void Fill_3(float SKrsdy1, float SKrsdy2,float SKrsdy3,float SKrsdy4,float SKrsdy5,float SKrsdy6) {

       hSKrsdy1->Fill(SKrsdy1);
       hSKrsdy2->Fill(SKrsdy2);
       hSKrsdy3->Fill(SKrsdy3);
       hSKrsdy4->Fill(SKrsdy4);
       hSKrsdy5->Fill(SKrsdy5);
       hSKrsdy6->Fill(SKrsdy6);
  }

  void Fill_4_3(float rsd_Rdphi, float rsd_RdphiWth, float rht_y) {
       hRdfRsd_Y1_3->Fill(rht_y,rsd_Rdphi);
       hRdfRsdWth_Y1_3->Fill(rht_y,rsd_RdphiWth);
  }
  void Fill_4_3P(float rsd_RdphiWthP) {
       hrsd_RdphiWthP->Fill(rsd_RdphiWthP);
  }
  void Fill_4_3N(float rsd_RdphiWthN) {
       hrsd_RdphiWthN->Fill(rsd_RdphiWthN);
  }

  void Fill_4a_3(float rsd_Rdphi, float rsd_RdphiWth, float rht_y) {
       hRdfRsd_Y2_3->Fill(rht_y,rsd_Rdphi);
       hRdfRsdWth_Y2_3->Fill(rht_y,rsd_RdphiWth);
  }

  void Fill_4b(float Rdf1, float y1,float Rdf2, float y2,float Rdf3, float y3,float Rdf4, float y4,float Rdf5, float y5,float Rdf6, float y6) {
       hRdf_Y_1->Fill(y1,Rdf1);
       hRdf_Y_2->Fill(y2,Rdf2);
       hRdf_Y_3->Fill(y3,Rdf3);
       hRdf_Y_4->Fill(y4,Rdf4);
       hRdf_Y_5->Fill(y5,Rdf5);
       hRdf_Y_6->Fill(y6,Rdf6);
  }
  void Fill_4c(float Rdf0, float RdfWth0, float y0, float XRec, float XFit) {
       hRdf_Y_0->Fill(y0,Rdf0);
       hRdfWth_Y_0->Fill(y0,RdfWth0);
       hXRec_XFit->Fill(XFit,XRec);
  }

  void Fill_5(float xpull, float rsd_Rdphi, int layer, float xRecErr, float slx1) {
       hxPull_CFEB0->Fill(layer, xpull);
       hrsd_Rdphi_CFEB0->Fill(layer, rsd_Rdphi);
       hErr_CFEB->Fill(layer, xRecErr);
       hSlope_CFEB->Fill(layer, slx1);
  }
  void Fill_6(float xpull, float rsd_Rdphi, int layer) {
       hxPull_CFEB1->Fill(layer, xpull);
       hrsd_Rdphi_CFEB1->Fill(layer, rsd_Rdphi);
  }


  void Write() {

    hSKrsdx1->Write();
    hSKrsdx2->Write();
    hSKrsdx3->Write();
    hSKrsdx4->Write();
    hSKrsdx5->Write();
    hSKrsdx6->Write();

    hSKrsdxWth1->Write();
    hSKrsdxWth2->Write();
    hSKrsdxWth3->Write();
    hSKrsdxWth4->Write();
    hSKrsdxWth5->Write();
    hSKrsdxWth6->Write();

    hxPull1->Write();
    hxPull2->Write();
    hxPull3->Write();
    hxPull4->Write();
    hxPull5->Write();
    hxPull6->Write();

    hSKrsdy1->Write();
    hSKrsdy2->Write();
    hSKrsdy3->Write();
    hSKrsdy4->Write();
    hSKrsdy5->Write();
    hSKrsdy6->Write();

    hrsd_Rdphi1->Write();
    hrsd_Rdphi2->Write();
    hrsd_Rdphi3->Write();
    hrsd_Rdphi4->Write();
    hrsd_Rdphi5->Write();
    hrsd_Rdphi6->Write();

    hrsd_RdphiWth1->Write();
    hrsd_RdphiWth2->Write();
    hrsd_RdphiWth3->Write();
    hrsd_RdphiWth4->Write();
    hrsd_RdphiWth5->Write();
    hrsd_RdphiWth6->Write();
  
    hrsd_RdphiWthP->Write();
    hrsd_RdphiWthN->Write();
  
    hRdfRsd_Y1_3->Write();
    hRdfRsd_Y2_3->Write();
    hRdfRsdWth_Y1_3->Write();
    hRdfRsdWth_Y2_3->Write();

    hRdf_Y_1->Write();
    hRdf_Y_2->Write();
    hRdf_Y_3->Write();
    hRdf_Y_4->Write();
    hRdf_Y_5->Write();
    hRdf_Y_6->Write();
    hRdf_Y_0->Write();
    hRdfWth_Y_0->Write();
    
    hXRec_XFit->Write();
    
    hxPull_CFEB0->Write();
    hrsd_Rdphi_CFEB0->Write();
    hxPull_CFEB1->Write();
    hrsd_Rdphi_CFEB1->Write();
    hErr_CFEB->Write();
    hSlope_CFEB->Write();

  }

    TH1F *hSKrsdx1;
    TH1F *hSKrsdx2;
    TH1F *hSKrsdx3;
    TH1F *hSKrsdx4;
    TH1F *hSKrsdx5;
    TH1F *hSKrsdx6;
 
    TH1F *hSKrsdxWth1;
    TH1F *hSKrsdxWth2;
    TH1F *hSKrsdxWth3;
    TH1F *hSKrsdxWth4;
    TH1F *hSKrsdxWth5;
    TH1F *hSKrsdxWth6;
 
    TH1F *hxPull1;
    TH1F *hxPull2;
    TH1F *hxPull3;
    TH1F *hxPull4;
    TH1F *hxPull5;
    TH1F *hxPull6;

    TH1F *hSKrsdy1;
    TH1F *hSKrsdy2;
    TH1F *hSKrsdy3;
    TH1F *hSKrsdy4;
    TH1F *hSKrsdy5;
    TH1F *hSKrsdy6;
 
    TH1F *hrsd_Rdphi1;
    TH1F *hrsd_Rdphi2;
    TH1F *hrsd_Rdphi3;
    TH1F *hrsd_Rdphi4;
    TH1F *hrsd_Rdphi5;
    TH1F *hrsd_Rdphi6;

    TH1F *hrsd_RdphiWth1;
    TH1F *hrsd_RdphiWth2;
    TH1F *hrsd_RdphiWth3;
    TH1F *hrsd_RdphiWth4;
    TH1F *hrsd_RdphiWth5;
    TH1F *hrsd_RdphiWth6;

    TH1F *hrsd_RdphiWthP;
    TH1F *hrsd_RdphiWthN;

    TH2F *hRdfRsd_Y1_3;
    TH2F *hRdfRsd_Y2_3;
    TH2F *hRdfRsdWth_Y1_3;
    TH2F *hRdfRsdWth_Y2_3;
    
    TH2F *hRdf_Y_1;
    TH2F *hRdf_Y_2;
    TH2F *hRdf_Y_3;
    TH2F *hRdf_Y_4;
    TH2F *hRdf_Y_5;
    TH2F *hRdf_Y_6;
    TH2F *hRdf_Y_0;
    TH2F *hRdfWth_Y_0;

    TH2F *hXRec_XFit;

    TH2F *hxPull_CFEB0;
    TH2F *hrsd_Rdphi_CFEB0; 
    TH2F *hxPull_CFEB1;
    TH2F *hrsd_Rdphi_CFEB1; 
    TH2F *hErr_CFEB;
    TH2F *hSlope_CFEB; 

  TString name;
};

class H2DRecHit2 {
public:

  H2DRecHit2(std::string name_) {
    TString N2 = name_.c_str();
    name=N2;

    hCh_Layer5 = new TH2F(N2+"_hCH_Layer5", "layer vs Channel# for 5 hits", 80, 1., 81., 7, 0., 7.);
    hCh_Layer6 = new TH2F(N2+"_hCH_Layer6", "layer vs Channel# for 6 hits", 80, 1., 81., 7, 0., 7.);
    hWg_Layer5 = new TH2F(N2+"_hWg_Layer5", "layer vs Wiregroup for 5 hits", 114, 0., 114., 7, 0., 7.);
    hWg_Layer6 = new TH2F(N2+"_hWg_Layer6", "layer vs Wiregroup for 6 hits", 114, 0., 114., 7, 0., 7.);
  }

  H2DRecHit2(TString name_, TFile* file) {
    name=name_;

    hCh_Layer5  = (TH2F *) file->Get(name+"_hCh_Layer5");
    hCh_Layer5  = (TH2F *) file->Get(name+"_hCh_Layer5");
    hWg_Layer6  = (TH2F *) file->Get(name+"_hWg_Layer6");
    hWg_Layer6  = (TH2F *) file->Get(name+"_hWg_Layer6");
  } 

  /// Destructor
  virtual ~H2DRecHit2() {
    delete hCh_Layer5;
    delete hCh_Layer6;
    delete hWg_Layer5;
    delete hWg_Layer6;
  }

  void Fill_A5(int layer, int ch_number, int wire_group) {
       hCh_Layer5->Fill(ch_number,layer);
       hWg_Layer5->Fill(wire_group,layer);
  }
  void Fill_A6(int layer, int ch_number, int wire_group) {
       hCh_Layer6->Fill(ch_number,layer);
       hWg_Layer6->Fill(wire_group,layer);
  }

  void Write() {
    hCh_Layer5->Write();
    hCh_Layer6->Write();
    hWg_Layer5->Write();
    hWg_Layer6->Write();
  }

    TH2F *hCh_Layer5;
    TH2F *hCh_Layer6;
    TH2F *hWg_Layer5;
    TH2F *hWg_Layer6;

  TString name;
};


#endif
