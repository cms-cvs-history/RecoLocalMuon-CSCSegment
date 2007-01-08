void CSCMTCCPlotter_X(int st, int rg, int cb){

  /* Macro to plot histograms produced by CSCRecHitReader.cc
   * You may need to update the TFile name, and will need to
   * input the segtype as shown below.
   *
   * Author:  Dominique Fortin - UCR
   */


float  nsigmas = 2.;  // Number of sigmas around mean to fit gaussian.  It uses 2 iterations 
                   // i.e. range is = [mu - nsigmas * sigma, mu + nsigmas * sigma]

TFile *file = TFile::Open("mtccplots_2625_TC_calib.root");
FILE *dfile = fopen("run2526.txt","a");

TString suffixps = ".eps";

char det_str[9];
char det_typ[6];
double mean[6]={0.};
double err[6]={0.};
float ly[6]={1.,2.,3.,4.,5.,6.}

 sprintf(det_str,"ME_%d_%d_%d",st,rg,cb);
 sprintf(det_typ,"ME_%d_%d",st,rg);
 TString segment = det_str;
 TString segtype = det_typ;
// Files for histogram output --> set suffixps to desired file type:  e.g. .eps, .jpg, ...

 TString plot01 = "rdphi_l1_"+segment+suffixps;
 TString plot02 = "rdphi_l2_"+segment+suffixps;
 TString plot03 = "rdphi_l3_"+segment+suffixps;
 TString plot04 = "rdphi_l4_"+segment+suffixps;
 TString plot05 = "rdphi_l5_"+segment+suffixps;
 TString plot06 = "rdphi_l6_"+segment+suffixps;
 TString plot07 = "pull_l1_"+segment+suffixps; 
 TString plot08 = "pull_l2_"+segment+suffixps; 
 TString plot09 = "pull_l3_"+segment+suffixps; 
 TString plot10 = "pull_l4_"+segment+suffixps; 
 TString plot11 = "pull_l5_"+segment+suffixps; 
 TString plot12 = "pull_l6_"+segment+suffixps;
 TString plot13 = "nhits_"+segtype+suffixps;
 TString plot14 = "nsegments_"+segtype+suffixps;
 TString plot15 = "rdphi_Y_"+segtype+suffixps;
 TString plot16 = "chi2_"+segtype+suffixps;
 TString plot17 = "rdphi_all_"+segtype+suffixps;
  

// ********************************************************************
// Pointers to histograms
// ********************************************************************

    hSKrsdx1    = (TH1F *) file->Get(segment+"_hSKrsdxWth1");
    hSKrsdx2    = (TH1F *) file->Get(segment+"_hSKrsdxWth2");
    hSKrsdx3    = (TH1F *) file->Get(segment+"_hSKrsdxWth3");
    hSKrsdx4    = (TH1F *) file->Get(segment+"_hSKrsdxWth4");
    hSKrsdx5    = (TH1F *) file->Get(segment+"_hSKrsdxWth5");
    hSKrsdx6    = (TH1F *) file->Get(segment+"_hSKrsdxWth6");

    hxPull1    = (TH1F *) file->Get(segment+"_hxPull1");
    hxPull2    = (TH1F *) file->Get(segment+"_hxPull2");
    hxPull3    = (TH1F *) file->Get(segment+"_hxPull3");
    hxPull4    = (TH1F *) file->Get(segment+"_hxPull4");
    hxPull5    = (TH1F *) file->Get(segment+"_hxPull5");
    hxPull6    = (TH1F *) file->Get(segment+"_hxPull6");

    hNHits     = (TH1F *) file->Get(segtype+"_hNHits");
    hNSeg1     = (TH1F *) file->Get(segtype+"_hNSeg1");
    hfrsd_Y    = (TH2F *) file->Get(segtype+"_hRdphiRsd_Y");

    hX2        = (TH1F *) file->Get(segtype+"_hX2c6hits");

// *****************************************************************
// A) r dphi (in units of strip width)
// *****************************************************************

// 1) x rsd 1
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdx1->SetTitle(segment);
 hSKrsdx1->GetXaxis()->SetTitle("r#Detla#phi_{1}/strip width");
 hSKrsdx1->GetYaxis()->SetTitle(" ");
 hSKrsdx1->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdx1->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hSKrsdx1->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 mean[0] = gaus->GetParameter(1);
 err[0] = gaus->GetParameter(2);
 fprintf (dfile, "%d %d %d %f %f\n",st,rg,cb,mean[0],err[0]);
 c1->Print(plot01);


// 2) x rsd 2
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdx2->SetTitle(segment);
 hSKrsdx2->GetXaxis()->SetTitle("r#Detla#phi_{2}/strip width");
 hSKrsdx2->GetYaxis()->SetTitle(" ");
 hSKrsdx2->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdx1->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hSKrsdx2->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 mean[1] = gaus->GetParameter(1);
 err[1] = gaus->GetParameter(2);
 fprintf (dfile, "%d %d %d %f %f\n",st,rg,cb,mean[1],err[1]);
 c1->Print(plot02);


// 3) x rsd 3
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdx3->SetTitle(segment);
 hSKrsdx3->GetXaxis()->SetTitle("r#Detla#phi_{3}/strip width");
 hSKrsdx3->GetYaxis()->SetTitle(" ");
 hSKrsdx3->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdx3->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hSKrsdx3->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 mean[2] = gaus->GetParameter(1);
 err[2] = gaus->GetParameter(2);
 fprintf (dfile, "%d %d %d %f %f\n",st,rg,cb,mean[2],err[2]);
 c1->Print(plot03);


// 4) x rsd 4
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdx4->SetTitle(segment);
 hSKrsdx4->GetXaxis()->SetTitle("r#Detla#phi_{4}/strip width");
 hSKrsdx4->GetYaxis()->SetTitle(" ");
 hSKrsdx4->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdx4->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hSKrsdx4->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 mean[3] = gaus->GetParameter(1);
 err[3] = gaus->GetParameter(2);
 fprintf (dfile, "%d %d %d %f %f\n",st,rg,cb,mean[3],err[3]);
 c1->Print(plot04);


// 5) x rsd 5
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdx5->SetTitle(segment);
 hSKrsdx5->GetXaxis()->SetTitle("r#Detla#phi_{5}/strip width");
 hSKrsdx5->GetYaxis()->SetTitle(" ");
 hSKrsdx5->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdx5->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hSKrsdx5->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 mean[4] = gaus->GetParameter(1);
 err[4] = gaus->GetParameter(2);
 fprintf (dfile, "%d %d %d %f %f\n",st,rg,cb,mean[4],err[4]);
 c1->Print(plot05);


// 6) x rsd 6
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdx6->SetTitle(segment);
 hSKrsdx6->GetXaxis()->SetTitle("r#Detla#phi_{1}/strip width");
 hSKrsdx6->GetYaxis()->SetTitle(" ");
 hSKrsdx6->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdx6->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hSKrsdx6->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 mean[5] = gaus->GetParameter(1);
 err[5] = gaus->GetParameter(2);
 fprintf (dfile, "%d %d %d %f %f\n",st,rg,cb,mean[5],err[5]);
 c1->Print(plot06);


 //  ****************************************************
 //  B) Pulls in local x
 //  ****************************************************


// 1)
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hxPull1->SetTitle(segment);
// hxPull1->Draw();
 hxPull1->GetXaxis()->SetTitle("#Delta x_{1}/#sigma x_{1}");
 hxPull1->GetYaxis()->SetTitle(" ");
 hxPull1->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hxPull1->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hxPull1->Fit("gaus","R","",low,hi);
 c1->Print(plot07);


// 2)
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hxPull2->SetTitle(segment);
// hxPull2->Draw();
 hxPull2->GetXaxis()->SetTitle("#Delta x_{2}/#sigma x_{2}");
 hxPull2->GetYaxis()->SetTitle(" ");
 hxPull2->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hxPull1->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hxPull2->Fit("gaus","R","",low,hi);
 c1->Print(plot08);


// 3)
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hxPull3->SetTitle(segment);
// hxPull3->Draw();
 hxPull3->GetXaxis()->SetTitle("#Delta x_{3}/#sigma x_{3}");
 hxPull3->GetYaxis()->SetTitle(" ");
 hxPull3->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hxPull3->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hxPull3->Fit("gaus","R","",low,hi);
 c1->Print(plot09);


// 4)
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hxPull4->SetTitle(segment);
// hxPull4->Draw();
 hxPull4->GetXaxis()->SetTitle("#Delta x_{4}/#sigma x_{4}");
 hxPull4->GetYaxis()->SetTitle(" ");
 hxPull4->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hxPull4->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hxPull4->Fit("gaus","R","",low,hi);
 c1->Print(plot10);


// 5)
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hxPull5->SetTitle(segment);
// hxPull5->Draw();
 hxPull5->GetXaxis()->SetTitle("#Delta x_{5}/#sigma x_{5}");
 hxPull5->GetYaxis()->SetTitle(" ");
 hxPull5->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hxPull5->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hxPull5->Fit("gaus","R","",low,hi);
 c1->Print(plot11);


// 6) 
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hxPull6->SetTitle(segment);
// hxPull6->Draw();
 hxPull6->GetXaxis()->SetTitle("#Delta x_{6}/#sigma x_{6}");
 hxPull6->GetYaxis()->SetTitle(" ");
 hxPull6->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hxPull6->Fit("gaus","R","",low,hi);
 par0 = gaus->GetParameter(0);
 par1 = gaus->GetParameter(1);
 par2 = gaus->GetParameter(2);
 cout << "********* Second fit *********" << endl;
 cout << "Parameters are: " << "P0: " << par0
      <<  " P1: " << par1 << " P2: " << par2 << endl;
 gStyle->SetOptStat(kTRUE);
 gStyle->SetOptFit(0111);
 low = par1 -nsigmas * par2;
 hi = par1 + nsigmas * par2;
 hxPull6->Fit("gaus","R","",low,hi);
 c1->Print(plot12);

 // **********************************************
 // Segment stats
 // **********************************************

// 13) Number of Hits/ segment
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);   
 c1->SetFillColor(10);
 hNHits->SetTitle(segtype);
 hNHits->Draw();
 hNHits->GetXaxis()->SetTitle("# hits/segment ");
 hNHits->GetYaxis()->SetTitle(" ");
 c1->Print(plot13);
 
// 14) Number of Segments / chamber
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hNSeg1->SetTitle(segtype);
 hNSeg1->Draw();
 hNSeg1->GetXaxis()->SetTitle("# segments/chamber ");
 hNSeg1->GetYaxis()->SetTitle(" ");   
 c1->Print(plot14);
 
// 15) Residual vs Y
 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hfrsd_Y->SetTitle(segtype);
 hfrsd_Y->Draw();
 hfrsd_Y->GetXaxis()->SetTitle("Y_{local} (cm) ");
 hfrsd_Y->GetYaxis()->SetTitle("r#Delta#phi/strip width ");
 c1->Print(plot15);

// 16) chi^2 for 6 hits  Segments 
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hX2->SetTitle(segtype);
 hX2->Draw();
 hX2->GetXaxis()->SetTitle("#chi^{2}");
 hX2->GetYaxis()->SetTitle(" ");   
 c1->Print(plot16);


 gStyle->SetOptStat(kFALSE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 
 Rdf_ly = new TGraphErrors(6,ly,err);
 Rdf_ly->SetMarkerColor(4);
 Rdf_ly->SetMarkerStyle(21);
 Rdf_ly->GetXaxis()->SetTitle(" layer # ");
 Rdf_ly->GetYaxis()->SetTitle("r#Delta#phi/strip width");

 Rdf_ly->Draw("AP");
 c1->Print(plot17);

 fclose(dfile);
 gROOT->ProcessLine(".q");

}
