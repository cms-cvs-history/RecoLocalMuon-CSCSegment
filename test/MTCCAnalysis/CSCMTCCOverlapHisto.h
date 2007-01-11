#ifndef CSCSegment_CSCMTCCOverlapHisto_H
#define CSCSegment_CSCMTCCOverlapHisto_H

/** \class CSCMTCCOverlapHisto
 *  Collection of histograms for overlapping CSC chambers in MTCC test.
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

class Histos {
  
 public:
  /// Constructor from collection name
  Histos(std::string name_) {
    TString N = name_.c_str();
    name=N;
    
    hDphi1      = new TH1F(N+"_hDphi1", N, 51, -0.051, 0.051);
    hlocalDx1   = new TH1F(N+"_hlocalDx1", N, 51, -10.2, 10.2);
    hlocalDy1   = new TH1F(N+"_hlocalDy1", N, 51, -20.4, 20.4);
    hDR1        = new TH1F(N+"_hDR1", N, 51, -20.4, 20.4);
    hDphi2      = new TH1F(N+"_hDphi2", N, 51, -0.051, 0.051);
    hlocalDx2   = new TH1F(N+"_hlocalDx2", N, 51, -10.2, 10.2);
    hlocalDy2   = new TH1F(N+"_hlocalDy2", N, 51, -20.4, 20.4);
    hDR2        = new TH1F(N+"_hDR2", N, 51, -20.4, 20.4);
    hTheta12 = new TH1F(N+"_hTheta12", N, 50, 0., 0.5);
  }

   
  /// Constructor from collection name and TFile.
  Histos(TString name_, TFile* file) {
    name=name_;

    hDphi1    = (TH1F *) file->Get(name+"_hDphi1");
    hlocalDx1 = (TH1F *) file->Get(name+"_hlocalDx1");  
    hlocalDy1 = (TH1F *) file->Get(name+"_hlocalDy1");  
    hDR1      = (TH1F *) file->Get(name+"_hDR1");  
    hDphi2    = (TH1F *) file->Get(name+"_hDphi2");  
    hlocalDx2 = (TH1F *) file->Get(name+"_hlocalDx2");  
    hlocalDy2 = (TH1F *) file->Get(name+"_hlocalDy2");  
    hDR2      = (TH1F *) file->Get(name+"_hDR2");  
    hTheta12  = (TH1F *) file->Get(name+"_hTheta12");     
  }

  /// Destructor
  virtual ~Histos() {
    delete hDphi1;
    delete hlocalDx1;
    delete hlocalDy1;
    delete hDR1;
    delete hDphi2;  
    delete hlocalDx2;
    delete hlocalDy2;
    delete hDR2;    
    delete hTheta12;    
  }	       


  /// Fill all the histos
  void Fill(float dphi1, float dx1, float dy1, float dR1, 
	    float dphi2, float dx2, float dy2, float dR2, float costheta12) {
    
    float theta12 = acos(costheta12); 
    hDphi1->Fill(dphi1);   
    hlocalDx1->Fill(dx1); 
    hlocalDy1->Fill(dy1); 
    hDR1->Fill(dR1);
    hDphi2->Fill(dphi2);
    hlocalDx1->Fill(dx2); 
    hlocalDy1->Fill(dy2); 
    hDR2->Fill(dR2);
    hTheta12->Fill(theta12);
  }

  /// Write all the histos to currently opened file
  void Write() {

   hDphi1->Write();        
   hlocalDx1->Write();    
   hlocalDy1->Write();    
   hDR1->Write();
   hDphi2->Write();     
   hlocalDx2->Write();    
   hlocalDy2->Write();    
   hDR2->Write();
   hTheta12->Write();
  }

  TH1F *hDphi1; 
  TH1F *hlocalDx1;
  TH1F *hlocalDy1;
  TH1F *hDR1;
  TH1F *hDphi2; 
  TH1F *hlocalDx2;
  TH1F *hlocalDy2;
  TH1F *hDR2;
  TH1F *hTheta12;
  
  TString name;
};
#endif
