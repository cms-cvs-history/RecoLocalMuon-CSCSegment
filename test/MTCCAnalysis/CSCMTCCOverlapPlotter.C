void CSCMTCCOverlapPlotter(int iRegion){

  /* Macro to plot histograms produced by CSCMTCCOverlap.cc
   * You may need to update the TFile name, and will need to
   * input the overlap region (oRegion) as shown below.
   *
   * Author:  Dominique Fortin - UCR
   */

  float nsigmas = 1.5;    // Number of sigmas around mean to fit gaussian.  It uses 2 iterations 
                          // i.e. range is = [mu - nsigmas * sigma, mu + nsigmas * sigma]

TFile *file = TFile::Open("overlap_4241_sk.root");

TString oRegion = "ME12_27_28";

if (iRegion == 2) oRegion = "ME12_28_29";
if (iRegion == 3) oRegion = "ME12_29_30";
if (iRegion == 4) oRegion = "ME12_30_31";
if (iRegion == 5) oRegion = "ME12_31_32";


// Files for histogram output --> set suffixps to desired file type:  e.g. .eps, .jpg, ...

TString suffixps = ".eps";

TString plot1 = "deltaX1_"+oRegion+suffixps;
TString plot2 = "deltaX2_"+oRegion+suffixps;
TString plot3 = "deltaY1_"+oRegion+suffixps;
TString plot4 = "deltaY2_"+oRegion+suffixps;
TString plot5 = "deltaR1_"+oRegion+suffixps;
TString plot6 = "deltaR2_"+oRegion+suffixps;
TString plot7 = "deltaPhi1_"+oRegion+suffixps;
TString plot8 = "deltaPhi2_"+oRegion+suffixps;
TString plot9 = "theta12_"+oRegion+suffixps;
TString plot10= "deltaX12_"+oRegion+suffixps;
TString plot11= "deltaY12_"+oRegion+suffixps;
TString plot12= "deltaR12_"+oRegion+suffixps;
TString plot13= "deltaPhi12_"+oRegion+suffixps;

// efficiency plots
TString eplot1 = "eff_raw"+suffixps;
TString eplot2 = "eff_6hit"+suffixps;
TString eplot3 = "eff_nocfeb"+suffixps;
TString eplot4 = "eff_layer"+suffixps;
TString eplot5 = "eff_layer_27"+suffixps;
TString eplot6 = "eff_layer_28"+suffixps;
TString eplot7 = "eff_layer_29"+suffixps;
TString eplot8 = "eff_layer_30"+suffixps;
TString eplot9 = "eff_layer_31"+suffixps;
TString eplot10 ="eff_layer_32"+suffixps;


// ********************************************************************
// Setting pointers to histograms
// ********************************************************************

hDeltaX1        = (TH1F *) file->Get(oRegion+"_hlocalDx1");
hDeltaX2        = (TH1F *) file->Get(oRegion+"_hlocalDx2");
hDeltaY1        = (TH1F *) file->Get(oRegion+"_hlocalDy1");
hDeltaY2        = (TH1F *) file->Get(oRegion+"_hlocalDy2");
hDeltaR1        = (TH1F *) file->Get(oRegion+"_hDR1");
hDeltaR2        = (TH1F *) file->Get(oRegion+"_hDR2");
hDeltaPhi1      = (TH1F *) file->Get(oRegion+"_hDphi1");
hDeltaPhi2      = (TH1F *) file->Get(oRegion+"_hDphi2");
hTheta12        = (TH1F *) file->Get(oRegion+"_hTheta12");
hDeltaX12       = (TH1F *) file->Get(oRegion+"_hlocalDx12");
hDeltaY12       = (TH1F *) file->Get(oRegion+"_hlocalDy12");
hDeltaR12       = (TH1F *) file->Get(oRegion+"_hDR12");
hDeltaPhi12     = (TH1F *) file->Get(oRegion+"_hDphi12");

// efficiencies

hsegeffRaw     = (TH1F *) file->Get("hsegeffRaw");
hsegeff        = (TH1F *) file->Get("hsegeff");
hsegeffCFEB    = (TH1F *) file->Get("hsegeffCFEB");
hlayeffall     = (TH1F *) file->Get("hlayeff");
hlayeff27      = (TH1F *) file->Get("hlayeff_27");
hlayeff28      = (TH1F *) file->Get("hlayeff_28");
hlayeff29      = (TH1F *) file->Get("hlayeff_29");
hlayeff30      = (TH1F *) file->Get("hlayeff_30");
hlayeff31      = (TH1F *) file->Get("hlayeff_31");
hlayeff32      = (TH1F *) file->Get("hlayeff_32");


// ********************************************************************
// Plotting
// ********************************************************************

 gStyle->SetOptStat(kTRUE);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaX1->SetTitle(oRegion);
 hDeltaX1->Draw();
 hDeltaX1->GetXaxis()->SetTitle("#Delta X_{12, local} (cm)");
 c1->Print(plot1);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaX2->SetTitle(oRegion);
 hDeltaX2->Draw();
 hDeltaX2->GetXaxis()->SetTitle("#Delta X_{21, local} (cm)");
 c1->Print(plot2);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaY1->SetTitle(oRegion);
 hDeltaY1->Draw();
 hDeltaY1->GetXaxis()->SetTitle("#Delta Y_{12, local} (cm)");
 c1->Print(plot3);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaY2->SetTitle(oRegion);
 hDeltaY2->Draw();
 hDeltaY2->GetXaxis()->SetTitle("#Delta Y_{21, local} (cm)");
 c1->Print(plot4);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaR1->SetTitle(oRegion);
 hDeltaR1->Draw();
 hDeltaR1->GetXaxis()->SetTitle("#Delta R_{12, global} (cm)");
 c1->Print(plot5);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaR2->SetTitle(oRegion);
 hDeltaR2->Draw();
 hDeltaR2->GetXaxis()->SetTitle("#Delta R_{21, global} (cm)");
 c1->Print(plot6);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaPhi1->SetTitle(oRegion);
 hDeltaPhi1->Draw();
 hDeltaPhi1->GetXaxis()->SetTitle("#Delta #phi_{12, global} (rad)");
 c1->Print(plot7);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaPhi2->SetTitle(oRegion);
 hDeltaPhi2->Draw();
 hDeltaPhi2->GetXaxis()->SetTitle("#Delta #phi_{21, global} (rad)");
 c1->Print(plot8);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hTheta12->SetTitle(oRegion);
 hTheta12->Draw();
 hTheta12->GetXaxis()->SetTitle("#theta_{12} (rad)");
 c1->Print(plot9);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaX12->SetTitle(oRegion);
 hDeltaX12->Draw();
 hDeltaX12->GetXaxis()->SetTitle("#Delta X_{12, local} (cm)");
 c1->Print(plot10);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaY12->SetTitle(oRegion);
 hDeltaY12->Draw();
 hDeltaY12->GetXaxis()->SetTitle("#Delta Y_{12, local} (cm)");
 c1->Print(plot11);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaR1->SetTitle(oRegion);
 hDeltaR1->Draw();
 hDeltaR1->GetXaxis()->SetTitle("#Delta R_{12, global} (cm)");
 c1->Print(plot12);

 TCanvas *c1 = new TCanvas("c1","");
 c1->SetFillColor(10);
 c1->SetFillColor(10);
 hDeltaPhi12->SetTitle(oRegion);
 hDeltaPhi12->Draw();
 hDeltaPhi12->GetXaxis()->SetTitle("#Delta #phi_{12, global} (rad)");   
 c1->Print(plot13);



// Efficiency plots

gStyle->SetOptStat(kFALSE);

TCanvas *c1 = new TCanvas("c1","");
c1->SetFillColor(10);
c1->SetFillColor(10);
hsegeffRaw->SetTitle("");
hsegeffRaw->Draw();
c1->Print(eplot1);

TCanvas *c1 = new TCanvas("c1","");
c1->SetFillColor(10);
c1->SetFillColor(10);
hsegeff->SetTitle("");
hsegeff->Draw();      
c1->Print(eplot2);

TCanvas *c1 = new TCanvas("c1","");
c1->SetFillColor(10);
c1->SetFillColor(10);
hsegeffCFEB->SetTitle("");
hsegeffCFEB->Draw();      
c1->Print(eplot3);

TCanvas *c1 = new TCanvas("c1","");
c1->SetFillColor(10);
c1->SetFillColor(10);
hlayeffall->SetTitle("");
hlayeffall->Draw();      
hlayeffall->GetXaxis()->SetTitle("layer id");
c1->Print(eplot4);

TCanvas *c1 = new TCanvas("c1","");
c1->SetFillColor(10);
c1->SetFillColor(10);
hlayeff27->SetTitle("ME12_27");
hlayeff27->Draw();
hlayeff27->GetXaxis()->SetTitle("layer id");
c1->Print(eplot5);

TCanvas *c1 = new TCanvas("c1","");
c1->SetFillColor(10);
c1->SetFillColor(10);
hlayeff28->SetTitle("ME12_28");
hlayeff28->Draw();
hlayeff28->GetXaxis()->SetTitle("layer id");
c1->Print(eplot6);

TCanvas *c1 = new TCanvas("c1","");
c1->SetFillColor(10);
c1->SetFillColor(10);
hlayeff29->SetTitle("ME12_29");
hlayeff29->Draw();
hlayeff29->GetXaxis()->SetTitle("layer id");
c1->Print(eplot7);

TCanvas *c1 = new TCanvas("c1","");
c1->SetFillColor(10);
c1->SetFillColor(10);
hlayeff30->SetTitle("ME12_30");
hlayeff30->Draw();
hlayeff30->GetXaxis()->SetTitle("layer id");
c1->Print(eplot8);

TCanvas *c1 = new TCanvas("c1","");
c1->SetFillColor(10);
c1->SetFillColor(10);
hlayeff31->SetTitle("ME12_31");
hlayeff31->Draw();
hlayeff31->GetXaxis()->SetTitle("layer id");
c1->Print(eplot9);

TCanvas *c1 = new TCanvas("c1","");
c1->SetFillColor(10);
c1->SetFillColor(10);
hlayeff32->SetTitle("ME12-32");
hlayeff32->Draw();
hlayeff32->GetXaxis()->SetTitle("layer id");
c1->Print(eplot10);


gROOT->ProcessLine(".q");

}


