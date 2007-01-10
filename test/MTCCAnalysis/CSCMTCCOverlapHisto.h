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
    hrDphi1     = new TH1F(N+"_hrDphi1", N, 51, -10.2, 10.2);
    hDR1        = new TH1F(N+"_hDR1", N, 51, -10.2, 10.2);
    hDphi2      = new TH1F(N+"_hDphi2", N, 51, -0.051, 0.051);
    hrDphi2     = new TH1F(N+"_hrDphi2", N, 51, -10.2, 10.2);
    hDR2        = new TH1F(N+"_hDR2", N, 51, -10.2, 10.2);
    hTheta12 = new TH1F(N+"_hTheta12", N, 51, -0.01, 0.5);
  }

   
  /// Constructor from collection name and TFile.
  Histos(TString name_, TFile* file) {
    name=name_;

    hDphi1  = (TH1F *) file->Get(name+"_hDphi1");
    hrDphi1 = (TH1F *) file->Get(name+"_hrDphi1");  
    hDR1    = (TH1F *) file->Get(name+"_hDR1");  
    hDphi2  = (TH1F *) file->Get(name+"_hDphi2");  
    hrDphi2 = (TH1F *) file->Get(name+"_hrDphi2");
    hDR2    = (TH1F *) file->Get(name+"_hDR2");  
    hTheta12 = (TH1F *) file->Get(name+"_hTheta12");     
  }

  /// Destructor
  virtual ~Histos() {
    delete hDphi1;          
    delete hrDphi1;
    delete hDR1;
    delete hDphi2;  
    delete hrDphi2;
    delete hDR2;    
    delete hTheta12;    
  }	       


  /// Fill all the histos
  void Fill(float dphi1, float rDphi1, float dR1, 
	    float dphi2, float rDphi2, float dR2, float costheta12) {
    
    float theta12 = acos(costheta12); 
    hDphi1->Fill(dphi1);   
    hrDphi1->Fill(rDphi1); 
    hDR1->Fill(dR1);
    hDphi2->Fill(dphi2);
    hrDphi2->Fill(rDphi2);
    hDR2->Fill(dR2);
    hTheta12->Fill(theta12);
  }

  /// Write all the histos to currently opened file
  void Write() {

   hDphi1->Write();        
   hrDphi1->Write();    
   hDR1->Write();
   hDphi2->Write();     
   hrDphi2->Write();
   hDR2->Write();
   hTheta12->Write();
  }

  TH1F *hDphi1; 
  TH1F *hrDphi1;
  TH1F *hDR1;
  TH1F *hDphi2; 
  TH1F *hrDphi2;
  TH1F *hDR2;
  TH1F *hTheta12;
  
  TString name;
};
#endif
