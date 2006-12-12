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
// Local X position    
    hOrgX    = new TH1F(N+"_hOrgX", " segment origin X", 320, -80., 80.);
// Local Y position
    hOrgY    = new TH1F(N+"_hOrgY", " segment origin Y", 400, -100., 100.);
// Chi2
    hX2      = new TH1F(N+"_hX2", " segment Chi Square", 160, 0., 80.);
// number of hits
    hNHits   = new TH1F(N+"_hNHits", " segment Hits number", 10, 0., 10.);
    hNSeg1 = new TH1F(N+"_hNSeg1", "# of segment/events ", 15, 0., 15.);
//

    hslx1      = new TH1F(N+"_hslx1", "slope from vector xz ", 500, -5., 5.);
    hxRsd_Y    = new TH2F(N+"_hxRsd_Y", "unbias x residual vs Y", 20, -160., 160., 240, -0.6, 0.6);
    hxRsdWth_Y = new TH2F(N+"_hxRsdWth_Y", "unbias x residual(strip width) vs Y", 20, -160., 160., 240, -0.6, 0.6);
    hRdphiRsd_Y= new TH2F(N+"_hRdphiRsd_Y", "unbias Rdphi residual vs Y", 20, -160., 160., 240, -0.6, 0.6);
    hxPull_Y   = new TH2F(N+"_hxPull_Y", " x Pull vs Y", 20, -160., 160., 105, -10., 10.);
    hxPull_X2  = new TH2F(N+"_hxPull_X2", " x Pull vs X2", 160, 0., 80., 105, -10., 10.);
    hxPull_slx = new TH2F(N+"_hxPull_slx", " x Pull vs slop x", 40, -1., 1., 105, -10., 10.);
    hxPull_sly = new TH2F(N+"_hxPull_sly", " x Pull vs slop y", 120, -3., 3.,105, -10., 10.);
    hxPull_err = new TH2F(N+"_hxPull_err", " x Pull vs err x", 300, 0., 0.03,105, -10., 10.);
    hxPull_dx  = new TH2F(N+"_hxPull_dx", " x Pull vs dx ", 240, -0.6, 0.6,105, -10., 10.);
    hdx_err    = new TH2F(N+"_hdx_err", " dx  vs err x ", 240, -0.6, 0.6,300, 0., 0.03);
// Errors
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
    hNSeg1  = (TH1F *) file->Get(name+"_hNSeg1");
//

    hslx1       = (TH1F *) file->Get(name+"_hslx1");
    hxRsd_Y     = (TH2F *) file->Get(name+"_hxRrsd_Y");
    hxRsdWth_Y  = (TH2F *) file->Get(name+"_hxRrsdWth_Y");
    hRdphiRsd_Y = (TH2F *) file->Get(name+"_hRdphiRrsd_Y");
    hxPull_Y    = (TH2F *) file->Get(name+"_hxPull_Y");
    hxPull_X2   = (TH2F *) file->Get(name+"_hxPull_X2");
    hxPull_slx  = (TH2F *) file->Get(name+"_hxPull_slx");
    hxPull_sly  = (TH2F *) file->Get(name+"_hxPull_sly");
    hxPull_err  = (TH2F *) file->Get(name+"_hxPull_err");
    hxPull_dx   = (TH2F *) file->Get(name+"_hxPull_dx");
    hdx_err     = (TH2F *) file->Get(name+"_hdx_err");

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
// X
    delete hOrgX;
// Y
    delete hOrgY;
// Chi2
    delete hX2;
// number of hits
    delete hNHits;
    delete hNSeg1;
// residual
    delete hslx1;
    delete hxRsd_Y;
    delete hxRsdWth_Y;
    delete hRdphiRsd_Y;
    delete hxPull_Y;
    delete hxPull_X2;
    delete hxPull_slx;
    delete hxPull_sly;
    delete hxPull_err;
    delete hxPull_dx;
    delete hdx_err;

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
  void Fill_c(float xorgc, float yorgc, double X2c, int nhitsc, float slx1) {
// X
    hOrgX->Fill(xorgc);
// Y  
    hOrgY->Fill(yorgc);
// Chi2
    hX2->Fill(X2c);
// # of hits
    hNHits->Fill(nhitsc);
// 
    hslx1->Fill(slx1);
  
  }

// Fill the segment and nhits info
  void Fill_a(int nu_seg1) {
       hNSeg1->Fill(nu_seg1);
  }

  void Fill_e(float xRsd, float xRsdWth, float rsd_Rdphi, float xPull, double X2, float slx1, float sly1,float xerr, float dx, float rht_y) {

       hxRsd_Y->Fill(rht_y,xRsd);
       hxRsdWth_Y->Fill(rht_y,xRsdWth);
       hRdphiRsd_Y->Fill(rht_y,rsd_Rdphi);
       hxPull_Y->Fill(rht_y,xPull);
       hxPull_X2->Fill(X2,xPull);
       hxPull_slx->Fill(slx1,xPull);
       hxPull_sly->Fill(sly1,xPull);
       hxPull_err->Fill(xerr,xPull);
       hxPull_dx->Fill(dx,xPull);
       hdx_err->Fill(dx,xerr);

  }

  void Fill_g(float Err1OrgX, float Err1OrgY, float Err1VecX, float Err1VecY, double X2c, float xRecErr0,float xRecErr2,float xRecErr4, float xFitErr0, float xFitErr2, float xFitErr4 ) {

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
    hxRsd_Y->Write();
    hxRsdWth_Y->Write();
    hRdphiRsd_Y->Write();
    hxPull_Y->Write();
    hxPull_X2->Write();
    hxPull_slx->Write();
    hxPull_sly->Write();
    hxPull_err->Write();
    hxPull_dx->Write();
    hdx_err->Write();

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
    TH2F *hxRsd_Y;
    TH2F *hxRsdWth_Y;
    TH2F *hRdphiRsd_Y;
    TH2F *hxPull_Y;
    TH2F *hxPull_X2;
    TH2F *hxPull_slx;
    TH2F *hxPull_sly;
    TH2F *hxPull_err;
    TH2F *hxPull_dx;
    TH2F *hdx_err;

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
    TString N1 = name_.c_str();
    name=N1;

    hrsdx1 = new TH1F(N1+"_hrsdx1", "x Pull in 1st layer", 101, -5., 5.);
    hrsdx2 = new TH1F(N1+"_hrsdx2", "x Pull in 2st layer", 101, -5., 5.);
    hrsdx3 = new TH1F(N1+"_hrsdx3", "x Pull in 3st layer", 101, -5., 5.);
    hrsdx4 = new TH1F(N1+"_hrsdx4", "x Pull in 4st layer", 101, -5., 5.);
    hrsdx5 = new TH1F(N1+"_hrsdx5", "x Pull in 5st layer", 101, -5., 5.);
    hrsdx6 = new TH1F(N1+"_hrsdx6", "x Pull in 6st layer", 101, -5., 5.);

  }

  H2DRecHit1(TString name_, TFile* file) {
    name=name_;

    hrsdx1    = (TH1F *) file->Get(name+"_hrsdx1");
    hrsdx2    = (TH1F *) file->Get(name+"_hrsdx2");
    hrsdx3    = (TH1F *) file->Get(name+"_hrsdx3");
    hrsdx4    = (TH1F *) file->Get(name+"_hrsdx4");
    hrsdx5    = (TH1F *) file->Get(name+"_hrsdx5");
    hrsdx6    = (TH1F *) file->Get(name+"_hrsdx6");

  } 

  /// Destructor
  virtual ~H2DRecHit1() {

    delete hrsdx1;
    delete hrsdx2;
    delete hrsdx3;
    delete hrsdx4;
    delete hrsdx5;
    delete hrsdx6;

  }

  void Fill_b(float rsdx1, float rsdx2,float rsdx3,float rsdx4,float rsdx5,float rsdx6) {

       hrsdx1->Fill(rsdx1);
       hrsdx2->Fill(rsdx2);
       hrsdx3->Fill(rsdx3);
       hrsdx4->Fill(rsdx4);
       hrsdx5->Fill(rsdx5);
       hrsdx6->Fill(rsdx6);
  } 


  void Write() {

    hrsdx1->Write();
    hrsdx2->Write();
    hrsdx3->Write();
    hrsdx4->Write();
    hrsdx5->Write();
    hrsdx6->Write();
  
  }

    TH1F *hrsdx1;
    TH1F *hrsdx2;
    TH1F *hrsdx3;
    TH1F *hrsdx4;
    TH1F *hrsdx5;
    TH1F *hrsdx6;
 
  TString name;
};


/*class H2DRecHit2 {
public:

  H2DRecHit2(std::string name_) {
    TString N2 = name_.c_str();
    name=N2;

    hfrsdx1 = new TH1F(N2+"_hfrsdx1", "unbias x residual in 1st layer", 200, -1., 1.);
    hfrsdx2 = new TH1F(N2+"_hfrsdx2", "unbias x residual in 2st layer", 200, -1., 1.);
    hfrsdx3 = new TH1F(N2+"_hfrsdx3", "unbias x residual in 3st layer", 200, -1., 1.);
    hfrsdx4 = new TH1F(N2+"_hfrsdx4", "unbias x residual in 4st layer", 200, -1., 1.);
    hfrsdx5 = new TH1F(N2+"_hfrsdx5", "unbias x residual in 5st layer", 200, -1., 1.);
    hfrsdx6 = new TH1F(N2+"_hfrsdx6", "unbias x residual in 6st layer", 200, -1., 1.);

    hdfSKrsd1 = new TH1F(N2+"_hdfSKrsd1", "unbias x dresidual in 1st layer", 201, -0.2, 0.2);
    hdfSKrsd2 = new TH1F(N2+"_hdfSKrsd2", "unbias x dresidual in 2st layer", 201, -0.2, 0.2);
    hdfSKrsd3 = new TH1F(N2+"_hdfSKrsd3", "unbias x dresidual in 3st layer", 201, -0.2, 0.2);
    hdfSKrsd4 = new TH1F(N2+"_hdfSKrsd4", "unbias x dresidual in 4st layer", 201, -0.2, 0.2);
    hdfSKrsd5 = new TH1F(N2+"_hdfSKrsd5", "unbias x dresidual in 5st layer", 201, -0.2, 0.2);
    hdfSKrsd6 = new TH1F(N2+"_hdfSKrsd6", "unbias x dresidual in 6st layer", 201, -0.2, 0.2);
  }

  H2DRecHit2(TString name_, TFile* file) {
    name=name_;

    hfrsdx1    = (TH1F *) file->Get(name+"_hfrsdx1");
    hfrsdx2    = (TH1F *) file->Get(name+"_hfrsdx2");
    hfrsdx3    = (TH1F *) file->Get(name+"_hfrsdx3");
    hfrsdx4    = (TH1F *) file->Get(name+"_hfrsdx4");
    hfrsdx5    = (TH1F *) file->Get(name+"_hfrsdx5");
    hfrsdx6    = (TH1F *) file->Get(name+"_hfrsdx6");

    hdfSKrsd1    = (TH1F *) file->Get(name+"_hdfSKrsd1");
    hdfSKrsd2    = (TH1F *) file->Get(name+"_hdfSKrsd2");
    hdfSKrsd3    = (TH1F *) file->Get(name+"_hdfSKrsd3");
    hdfSKrsd4    = (TH1F *) file->Get(name+"_hdfSKrsd4");
    hdfSKrsd5    = (TH1F *) file->Get(name+"_hdfSKrsd5");
    hdfSKrsd6    = (TH1F *) file->Get(name+"_hdfSKrsd6");
  } 

  /// Destructor
  virtual ~H2DRecHit2() {

    delete hfrsdx1;
    delete hfrsdx2;
    delete hfrsdx3;
    delete hfrsdx4;
    delete hfrsdx5;
    delete hfrsdx6;
   
    delete hdfSKrsd1;
    delete hdfSKrsd2;
    delete hdfSKrsd3;
    delete hdfSKrsd4;
    delete hdfSKrsd5;
    delete hdfSKrsd6;
  }

  void Fill_d(float frsdx1, float frsdx2,float frsdx3,float frsdx4,float frsdx5,float frsdx6, float dfrsd1, float dfrsd2,float dfrsd3,float dfrsd4,float dfrsd5,float dfrsd6) {

       hfrsdx1->Fill(frsdx1);
       hfrsdx2->Fill(frsdx2);
       hfrsdx3->Fill(frsdx3);
       hfrsdx4->Fill(frsdx4);
       hfrsdx5->Fill(frsdx5);
       hfrsdx6->Fill(frsdx6);

       hdfSKrsd1->Fill(dfrsd1);
       hdfSKrsd2->Fill(dfrsd2);
       hdfSKrsd3->Fill(dfrsd3);
       hdfSKrsd4->Fill(dfrsd4);
       hdfSKrsd5->Fill(dfrsd5);
       hdfSKrsd6->Fill(dfrsd6);
  }

  void Write() {

    hfrsdx1->Write();
    hfrsdx2->Write();
    hfrsdx3->Write();
    hfrsdx4->Write();
    hfrsdx5->Write();
    hfrsdx6->Write();

    hdfSKrsd1->Write();
    hdfSKrsd2->Write();
    hdfSKrsd3->Write();
    hdfSKrsd4->Write();
    hdfSKrsd5->Write();
    hdfSKrsd6->Write();
  }

    TH1F *hfrsdx1;
    TH1F *hfrsdx2;
    TH1F *hfrsdx3;
    TH1F *hfrsdx4;
    TH1F *hfrsdx5;
    TH1F *hfrsdx6;

    TH1F *hdfSKrsd1;
    TH1F *hdfSKrsd2;
    TH1F *hdfSKrsd3;
    TH1F *hdfSKrsd4;
    TH1F *hdfSKrsd5;
    TH1F *hdfSKrsd6;
  TString name;
};*/

class H2DRecHit3 {
public:

  H2DRecHit3(std::string name_) {
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

    hSKrsdy1 = new TH1F(N3+"_hSKrsdy1", " y residual in 1st layer", 101, -5.01, 5.01);
    hSKrsdy2 = new TH1F(N3+"_hSKrsdy2", " y residual in 2st layer", 101, -5.01, 5.01);
    hSKrsdy3 = new TH1F(N3+"_hSKrsdy3", " y residual in 3st layer", 101, -5.01, 5.01);
    hSKrsdy4 = new TH1F(N3+"_hSKrsdy4", " y residual in 4st layer", 101, -5.01, 5.01);
    hSKrsdy5 = new TH1F(N3+"_hSKrsdy5", " y residual in 5st layer", 101, -5.01, 5.01);
    hSKrsdy6 = new TH1F(N3+"_hSKrsdy6", " y residual in 6st layer", 101, -5.01, 5.01);

    hrsd_Rdphi1 = new TH1F(N3+"_hrsd_Rdphi1", " Rdphi residual in 1st layer", 101, -0.3, 0.3);
    hrsd_Rdphi2 = new TH1F(N3+"_hrsd_Rdphi2", " Rdphi residual in 2st layer", 101, -0.3, 0.3);
    hrsd_Rdphi3 = new TH1F(N3+"_hrsd_Rdphi3", " Rdphi residual in 3st layer", 101, -0.3, 0.3);
    hrsd_Rdphi4 = new TH1F(N3+"_hrsd_Rdphi4", " Rdphi residual in 4st layer", 101, -0.3, 0.3);
    hrsd_Rdphi5 = new TH1F(N3+"_hrsd_Rdphi5", " Rdphi residual in 5st layer", 101, -0.3, 0.3);
    hrsd_Rdphi6 = new TH1F(N3+"_hrsd_Rdphi6", " Rdphi residual in 6st layer", 101, -0.3, 0.3);

  }

  H2DRecHit3(TString name_, TFile* file) {
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

  } 

  /// Destructor
  virtual ~H2DRecHit3() {

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

  }

  void Fill_f(float SKrsdx1, float SKrsdx2,float SKrsdx3,float SKrsdx4,float SKrsdx5,float SKrsdx6,float SKrsdxWth1, float SKrsdxWth2,float SKrsdxWth3,float SKrsdxWth4,float SKrsdxWth5,float SKrsdxWth6, float xPull1, float xPull2,float xPull3,float xPull4,float xPull5,float xPull6) {

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

       hxPull1->Fill(xPull1);
       hxPull2->Fill(xPull2);
       hxPull3->Fill(xPull3);
       hxPull4->Fill(xPull4);
       hxPull5->Fill(xPull5);
       hxPull6->Fill(xPull6);
  }

  void Fill_i(float SKrsdy1, float SKrsdy2,float SKrsdy3,float SKrsdy4,float SKrsdy5,float SKrsdy6,float rsd_Rdphi1, float rsd_Rdphi2,float rsd_Rdphi3,float rsd_Rdphi4,float rsd_Rdphi5,float rsd_Rdphi6) {

       hSKrsdy1->Fill(SKrsdy1);
       hSKrsdy2->Fill(SKrsdy2);
       hSKrsdy3->Fill(SKrsdy3);
       hSKrsdy4->Fill(SKrsdy4);
       hSKrsdy5->Fill(SKrsdy5);
       hSKrsdy6->Fill(SKrsdy6);

       hrsd_Rdphi1->Fill(rsd_Rdphi1);
       hrsd_Rdphi2->Fill(rsd_Rdphi2);
       hrsd_Rdphi3->Fill(rsd_Rdphi3);
       hrsd_Rdphi4->Fill(rsd_Rdphi4);
       hrsd_Rdphi5->Fill(rsd_Rdphi5);
       hrsd_Rdphi6->Fill(rsd_Rdphi6);
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

  TString name;
};
#endif


