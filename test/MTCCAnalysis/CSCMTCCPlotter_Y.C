void CSCMTCCPlotter1_Y(int st, int rg, int cb){

  /* Macro to plot histograms produced by CSCRecHitReader.cc
   * You may need to update the TFile name, and will need to
   * input the segtype as shown below.
   *
   * Author:  Dominique Fortin - UCR
   */


float  nsigmas = 1.25;  // Number of sigmas around mean to fit gaussian.  It uses 2 iterations 
                   // i.e. range is = [mu - nsigmas * sigma, mu + nsigmas * sigma]

TFile *file = TFile::Open("mtccplots_4241_DF_uc.root");

TString suffixps = ".jpg";

char det_str[9];
char det_typ[6];
 sprintf(det_str,"ME_%d_%d_%d",st,rg,cb);
 sprintf(det_typ,"ME_%d_%d",st,rg);
 TString segment = det_str;
 TString segtype = det_typ;
// Files for histogram output --> set suffixps to desired file type:  e.g. .eps, .jpg, ...

 TString plot01 = "ResidualY1_"+segment+suffixps;
 TString plot02 = "ResidualY2_"+segment+suffixps;
 TString plot03 = "ResidualY3_"+segment+suffixps;
 TString plot04 = "ResidualY4_"+segment+suffixps;
 TString plot05 = "ResidualY5_"+segment+suffixps;
 TString plot06 = "ResidualY6_"+segment+suffixps;
 TString plot07 = "Residual_Rdphi1_"+segment+suffixps; 
 TString plot08 = "Residual_Rdphi2_"+segment+suffixps; 
 TString plot09 = "Residual_Rdphi3_"+segment+suffixps; 
 TString plot10 = "Residual_Rdphi4_"+segment+suffixps; 
 TString plot11 = "Residual_Rdphi5_"+segment+suffixps; 
 TString plot12 = "Residual_Rdphi6_"+segment+suffixps;
 //TString plot16 = "X2c6hits_"+segtype+suffixps;
 //TString plot17 = "Err1OrgX_"+segtype+suffixps;
 //TString plot18 = "Err1VecX_"+segtype+suffixps;
 //TString plot19 = "Err3OrgX_"+segtype+suffixps;
 //TString plot20 = "Err3VecX_"+segtype+suffixps;
  

// ********************************************************************
// Pointers to histograms
// ********************************************************************

    hSKrsdy1    = (TH1F *) file->Get(segment+"_hSKrsdy1");
    hSKrsdy2    = (TH1F *) file->Get(segment+"_hSKrsdy2");
    hSKrsdy3    = (TH1F *) file->Get(segment+"_hSKrsdy3");
    hSKrsdy4    = (TH1F *) file->Get(segment+"_hSKrsdy4");
    hSKrsdy5    = (TH1F *) file->Get(segment+"_hSKrsdy5");
    hSKrsdy6    = (TH1F *) file->Get(segment+"_hSKrsdy6");

    hrsd_Rdphi1    = (TH1F *) file->Get(segment+"_hrsd_Rdphi1");
    hrsd_Rdphi2    = (TH1F *) file->Get(segment+"_hrsd_Rdphi2");
    hrsd_Rdphi3    = (TH1F *) file->Get(segment+"_hrsd_Rdphi3");
    hrsd_Rdphi4    = (TH1F *) file->Get(segment+"_hrsd_Rdphi4");
    hrsd_Rdphi5    = (TH1F *) file->Get(segment+"_hrsd_Rdphi5");
    hrsd_Rdphi6    = (TH1F *) file->Get(segment+"_hrsd_Rdphi6");


//    hX2        = (TH1F *) file->Get(segment+"_hX2");
//    hErr1OrgX  = (TH1F *) file->Get(segment+"_hErr1OrgX");
//    hErr1VecX  = (TH1F *) file->Get(segment+"_hErr1VecX");
//    hErr3OrgX  = (TH1F *) file->Get(segment+"_hErr3OrgX");
//    hErr3VecX  = (TH1F *) file->Get(segment+"_hErr3VecX");

// *****************************************************************
// 1) Local X position
// *****************************************************************

// 1) x rsd 1
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdy1->SetTitle(segment);
// hSKrsdy1->Draw();
 hSKrsdy1->GetXaxis()->SetTitle("y residual of layer1 (cm)");
 hSKrsdy1->GetYaxis()->SetTitle(" ");
 hSKrsdy1->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdy1->Fit("gaus","R","",low,hi);
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
 hSKrsdy1->Fit("gaus","R","",low,hi);
// hResPhi->Draw();
 c1->Print(plot01);


// 2) x rsd 2
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdy2->SetTitle(segment);
// hSKrsdy2->Draw();
 hSKrsdy2->GetXaxis()->SetTitle("y residual of layer2 (cm)");
 hSKrsdy2->GetYaxis()->SetTitle(" ");
 hSKrsdy2->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdy1->Fit("gaus","R","",low,hi);
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
 hSKrsdy2->Fit("gaus","R","",low,hi);
// hResPhi->Draw();
 c1->Print(plot02);


// 3) x rsd 3
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdy3->SetTitle(segment);
// hSKrsdy3->Draw();
 hSKrsdy3->GetXaxis()->SetTitle("y residual of layer3 (cm)");
 hSKrsdy3->GetYaxis()->SetTitle(" ");
 hSKrsdy3->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdy3->Fit("gaus","R","",low,hi);
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
 hSKrsdy3->Fit("gaus","R","",low,hi);
 c1->Print(plot03);


// 4) x rsd 4
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdy4->SetTitle(segment);
// hSKrsdy4->Draw();
 hSKrsdy4->GetXaxis()->SetTitle("y residual of layer4 (cm)");
 hSKrsdy4->GetYaxis()->SetTitle(" ");
 hSKrsdy4->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdy4->Fit("gaus","R","",low,hi);
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
 hSKrsdy4->Fit("gaus","R","",low,hi);
 c1->Print(plot04);


// 5) x rsd 5
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdy5->SetTitle(segment);
// hSKrsdy5->Draw();
 hSKrsdy5->GetXaxis()->SetTitle("y residual of layer5 (cm)");
 hSKrsdy5->GetYaxis()->SetTitle(" ");
 hSKrsdy5->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdy5->Fit("gaus","R","",low,hi);
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
 hSKrsdy5->Fit("gaus","R","",low,hi);
 c1->Print(plot05);


// 6) x rsd 6
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hSKrsdy6->SetTitle(segment);
// hSKrsdy6->Draw();
 hSKrsdy6->GetXaxis()->SetTitle("y residual of layer6 (cm)");
 hSKrsdy6->GetYaxis()->SetTitle(" ");
 hSKrsdy6->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hSKrsdy6->Fit("gaus","R","",low,hi);
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
 hSKrsdy6->Fit("gaus","R","",low,hi);
// hResPhi->Draw();
 c1->Print(plot06);


// 7) Residual Rdphi 7
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrsd_Rdphi1->SetTitle(segment);
// hrsd_Rdphi1->Draw();
 hrsd_Rdphi1->GetXaxis()->SetTitle("Residual Rdphi of layer1 (cm)");
 hrsd_Rdphi1->GetYaxis()->SetTitle(" ");
 hrsd_Rdphi1->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hrsd_Rdphi1->Fit("gaus","R","",low,hi);
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
 hrsd_Rdphi1->Fit("gaus","R","",low,hi);
 c1->Print(plot07);


// 8) Residual Rdphi 8
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrsd_Rdphi2->SetTitle(segment);
// hrsd_Rdphi2->Draw();
 hrsd_Rdphi2->GetXaxis()->SetTitle("Residual Rdphi of layer2 (cm)");
 hrsd_Rdphi2->GetYaxis()->SetTitle(" ");
 hrsd_Rdphi2->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hrsd_Rdphi1->Fit("gaus","R","",low,hi);
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
 hrsd_Rdphi2->Fit("gaus","R","",low,hi);
 c1->Print(plot08);


// 9) Residual Rdphi 9
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrsd_Rdphi3->SetTitle(segment);
// hrsd_Rdphi3->Draw();
 hrsd_Rdphi3->GetXaxis()->SetTitle("Residual Rdphi of layer3 (cm)");
 hrsd_Rdphi3->GetYaxis()->SetTitle(" ");
 hrsd_Rdphi3->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hrsd_Rdphi3->Fit("gaus","R","",low,hi);
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
 hrsd_Rdphi3->Fit("gaus","R","",low,hi);
 c1->Print(plot09);


// 10) Residual Rdphi 10
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrsd_Rdphi4->SetTitle(segment);
// hrsd_Rdphi4->Draw();
 hrsd_Rdphi4->GetXaxis()->SetTitle("Residual Rdphi of layer4 (cm)");
 hrsd_Rdphi4->GetYaxis()->SetTitle(" ");
 hrsd_Rdphi4->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hrsd_Rdphi4->Fit("gaus","R","",low,hi);
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
 hrsd_Rdphi4->Fit("gaus","R","",low,hi);
 c1->Print(plot10);


// 11) Residual Rdphi 11
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrsd_Rdphi5->SetTitle(segment);
// hrsd_Rdphi5->Draw();
 hrsd_Rdphi5->GetXaxis()->SetTitle("Residual Rdphi of layer5 (cm)");
 hrsd_Rdphi5->GetYaxis()->SetTitle(" ");
 hrsd_Rdphi5->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hrsd_Rdphi5->Fit("gaus","R","",low,hi);
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
 hrsd_Rdphi5->Fit("gaus","R","",low,hi);
 c1->Print(plot11);


// 12) Residual Rdphi 12
 gStyle->SetOptStat(kFALSE);  
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hrsd_Rdphi6->SetTitle(segment);
// hrsd_Rdphi6->Draw();
 hrsd_Rdphi6->GetXaxis()->SetTitle("Residual Rdphi of layer6 (cm)");
 hrsd_Rdphi6->GetYaxis()->SetTitle(" ");
 hrsd_Rdphi6->Fit("gaus");
 float par0 = gaus->GetParameter(0);
 float par1 = gaus->GetParameter(1);
 float par2 = gaus->GetParameter(2);
 cout << "Parameters are: " << "P0: " << par0
     <<  " P1: " << par1 << " P2: " <<par2 << endl;
 float low = par1 -nsigmas * par2;
 float hi = par1 + nsigmas * par2;
 hrsd_Rdphi6->Fit("gaus","R","",low,hi);
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
 hrsd_Rdphi6->Fit("gaus","R","",low,hi);
 c1->Print(plot12);

/*// 16) X2 for 6 hits  Segments 
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hX2->SetTitle(segtype);
 hX2->Draw();
 hX2->GetXaxis()->SetTitle(" Chi^2 ");
 hX2->GetYaxis()->SetTitle(" ");   
 c1->Print(plot16);


// 17) Number of Segments / chamber
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hErr1OrgX->SetTitle(segtype);
 hErr1OrgX->Draw();
 hErr1OrgX->GetXaxis()->SetTitle(" X Errors of Segment Vector ");
 hErr1OrgX->GetYaxis()->SetTitle(" ");   
 c1->Print(plot17);

// 18) Number of Segments / chamber
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hErr1VecX->SetTitle(segtype);
 hErr1VecX->Draw();
 hErr1VecX->GetXaxis()->SetTitle(" X Errors of Segment Origin ");
 hErr1VecX->GetYaxis()->SetTitle(" ");   
 c1->Print(plot18);

// 19) Number of Segments / chamber
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hErr3OrgX->SetTitle(segtype);
 hErr3OrgX->Draw();
 hErr3OrgX->GetXaxis()->SetTitle(" X Errors of Segment Origin ");
 hErr3OrgX->GetYaxis()->SetTitle(" ");   
 c1->Print(plot19);

// 20) Number of Segments / chamber
 gStyle->SetOptStat(kTRUE);
 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hErr3VecX->SetTitle(segtype);
 hErr3VecX->Draw();
 hErr3VecX->GetXaxis()->SetTitle(" X Errors of Segment Vector ");
 hErr3VecX->GetYaxis()->SetTitle(" ");   
 c1->Print(plot20);
*/

 gROOT->ProcessLine(".q");

}
