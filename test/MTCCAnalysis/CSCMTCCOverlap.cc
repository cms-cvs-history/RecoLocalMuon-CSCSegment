/*
 *  EDAnalyzer class which accesses CSCSegment and look at overlapping 
 *  chambers
 *
 *  Author: Dominique Fortin - UCR
 */

#include "CSCMTCCOverlap.h"

#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include <Geometry/CSCGeometry/interface/CSCChamber.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCLayerGeometry.h>
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include "Geometry/Vector/interface/GlobalPoint.h"
#include "Geometry/Vector/interface/GlobalVector.h"
#include "Geometry/Vector/interface/LocalPoint.h"
#include "Geometry/Vector/interface/LocalVector.h"
 
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "TFile.h"
#include "TH1F.h"
#include "TVector3.h"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <algorithm>

using namespace std;
using namespace edm;


// Constructor
CSCMTCCOverlap::CSCMTCCOverlap(const ParameterSet& pset) {

  // Get the various input parameters
  debug               = pset.getUntrackedParameter<bool>("debug");
  rootFileName        = pset.getUntrackedParameter<string>("rootFileName");
  recHitLabel         = pset.getUntrackedParameter<string>("recHitLabel");
  cscSegmentLabel     = pset.getUntrackedParameter<string>("cscSegmentLabel");
  maxdxdz             = pset.getUntrackedParameter<double>("maxSlopedxdz");
  maxdydz             = pset.getUntrackedParameter<double>("maxSlopedydz");
  minnhits            = pset.getUntrackedParameter<int>("MinNhits");
  minnhits2           = pset.getUntrackedParameter<int>("MinNhits2");
  maxDphi             = pset.getUntrackedParameter<double>("maxDphi");
  maxDR               = pset.getUntrackedParameter<double>("maxDR");
  minCosTheta12       = pset.getUntrackedParameter<double>("minCosTheta12");
  ChamberMarginAtEdges= pset.getUntrackedParameter<double>("ChamberMarginAtEdges");

  cout << "[CSCMTCCOverlap] Constructor called" << endl;
 
  // Create the root file
  theFile = new TFile(rootFileName.c_str(), "RECREATE");

  // Book the histograms
  h1  = new Histos("ME12_27_28");
  h2  = new Histos("ME12_28_29");
  h3  = new Histos("ME12_29_30");
  h4  = new Histos("ME12_30_31"); 
  h5  = new Histos("ME12_31_32");  // Note that 32 isn't calibrated !!!

  for (int chIdx = 0; chIdx < 7; chIdx++ ) segCount[chIdx] = 0;
  
  segCount5 = 0;
  OversegCount = 0;
  OversegCount5 = 0;  
  Noverlaps = 0;

}


// Destructor
CSCMTCCOverlap::~CSCMTCCOverlap(){

  cout << "[CSCMTCCOverlap] Destructor called " << endl;
  cout << "Number of segment pairs for overlapping region is " << Noverlaps << endl;  

  // Create histograms for rechit/segment efficiency for each ME-1/2 chambers:

  hlayeff[0] = new TH1F("hlayeff", "6-hit segment eff", layMap[0].size() + 2, 0, layMap[0].size() + 2); 
  // Begin loop histograms for all types of chambers
  char chName[9];
  int chIdx = 1;
  for (int ch = 27; ch < 33; ch++) {
    sprintf(chName,"hlayeff_%d",ch);
    hlayeff[chIdx] = new TH1F(chName, "6-hit segment eff", layMap[0].size() + 2, 0, layMap[0].size() + 2);
    chIdx++;
  }

  hsegeffRaw = new TH1F("hsegeffRaw", "Raw 6-hit segment eff", refMapRaw.size()*2 + 2, 0, refMapRaw.size()*2 + 2); 
  hsegeff = new TH1F("hsegeff", "6-hit segment eff", refMap.size()*2 + 2, 0, refMap.size()*2 + 2); 
  hsegeffCFEB = new TH1F("hsegeffCFEB", "6-hit segment eff", refMapCFEB.size()*2 + 2, 0, refMapCFEB.size()*2 + 2); 

  cout << "Rechit efficiency for building hit on segment/layer" << endl;
  int ibin;
  for (chIdx = 0; chIdx < 7; chIdx++ ) {
    ibin = 0;
    for (map<int,int>::const_iterator it = layMap[chIdx].begin(); it != layMap[chIdx].end(); it++) {
      ibin++;    
      float eff = 0.;
      if (segCount[chIdx] >0) eff = 1.* (float)it->second / segCount[chIdx]; 
      hlayeff[chIdx]->SetBinContent(ibin, eff);
      if ( chIdx == 0 ) cout << "Layer" << it->first << "  : " << it->second << " " << segCount[chIdx] << "  " << eff << endl;
    }
  }

  ibin = 0;
  cout << "Rechit efficiency for building hit from 5 hit segment" << endl;
  for (map<int,int>::const_iterator it = layMap5.begin(); it != layMap5.end(); it++) {
    ibin++;
    float eff = 0.;
    if (segCount5 >0) eff = 1.* (float)it->second / segCount5;
    cout << "Layer" << it->first << "  : " << it->second << " " << segCount5 << "  " << eff << endl;
  }

  cout << "Raw efficiency for building 6-hit segment" << endl;
  ibin = 0;
  for (map<int,int>::const_iterator it = segMapRaw.begin(); it != segMapRaw.end(); it++) {
    ibin++;
    float eff = (float)it->second/(float)refMapRaw[it->first];
    hsegeffRaw->SetBinContent(ibin*2, eff);
    char chName2[8];
    sprintf(chName2,"ME1/2-%d",it->first);
    std::string myCh = string(chName2);
    hsegeffRaw->GetXaxis()->SetBinLabel(ibin*2, chName2);
    cout << "Chamber" << it->first << ": " << it->second << " " << refMapRaw[it->first]  << "  " << eff << endl;
  }
  
  cout << "Efficiency for building 6-hit segment" << endl;
  ibin = 0;
  for (map<int,int>::const_iterator it = segMap.begin(); it != segMap.end(); it++) {
    ibin++;
    float eff = (float)it->second/(float)refMap[it->first];
    hsegeff->SetBinContent(ibin*2, eff);
    char chName2[8];
    sprintf(chName2,"ME1/2-%d",it->first);
    std::string myCh = string(chName2);
    hsegeff->GetXaxis()->SetBinLabel(ibin*2, chName2);
    cout << "Chamber" << it->first << ": " << it->second << " " << refMap[it->first]  << "  " << eff << endl;
  }

  cout << "Efficiency for building 6-hit segment (away from CFEB boundaries)" << endl;  
  ibin = 0;
  for (map<int,int>::const_iterator it = segMapCFEB.begin(); it != segMapCFEB.end(); it++) {
    ibin++;
    float eff = (float)it->second/(float)refMapCFEB[it->first];   
    hsegeffCFEB->SetBinContent(ibin*2, eff);
    char chName2[8];
    sprintf(chName2,"ME1/2-%d",it->first);
    std::string myCh = string(chName2);
    hsegeffCFEB->GetXaxis()->SetBinLabel(ibin*2, chName2);
    cout << "Chamber" << it->first << ": " << it->second << " " << refMapCFEB[it->first]  << "  " << eff << endl;
  }
    
  
  cout << " " << endl;
  cout << "*** Results from overlapping chambers " << endl;
  cout << " " << endl;

  // Overlapping chambers:  hit + segment reconstruction efficiency

  ibin = 0;
  cout << "Rechit efficiency for building hit on segment/layer" << endl;        
  for (map<int,int>::const_iterator it = OverlayMap.begin(); it != OverlayMap.end(); it++) {
    ibin++;
    float eff = 0.;
    if (OversegCount >0) eff = 1.* (float)it->second / OversegCount; 
    cout << "Layer" << it->first << "  : " << it->second << " " << OversegCount << "  " << eff << endl;
  }
  ibin = 0;
  cout << "Rechit efficiency for building hit on 5-hit segment/layer" << endl;
  for (map<int,int>::const_iterator it = OverlayMap5.begin(); it != OverlayMap5.end(); it++) {
    ibin++;
    float eff = 0.;
    if (OversegCount5 >0) eff = 1.* (float)it->second / OversegCount5;
    cout << "Layer" << it->first << "  : " << it->second << " " << OversegCount5 << "  " << eff << endl;
  }


  // Write out to file:
  
  theFile->cd();
  for (chIdx = 0; chIdx < 7; chIdx++) hlayeff[chIdx]->Write();  
  hsegeffRaw->Write();
  hsegeff->Write();
  hsegeffCFEB->Write();
  h1->Write();
  h2->Write();
  h3->Write();
  h4->Write();
  
  theFile->Close();

  if (debug) cout << "[CSCMTCCOverlap] Finished writing histograms to file" << endl;  
}



// The main
void CSCMTCCOverlap::analyze(const Event & event, const EventSetup& eventSetup){
   
  // Get the CSC Geometry :
  ESHandle<CSCGeometry> cscGeom;
  eventSetup.get<MuonGeometryRecord>().get(cscGeom);
  
  // Get the Segments collection :
  Handle<CSCSegmentCollection> cscSegments; 
  event.getByLabel(cscSegmentLabel, cscSegments);  
  
  // Get the RecHits collection :
  Handle<CSCRecHit2DCollection> recHits; 
  event.getByLabel(recHitLabel, recHits);  
  

  // First loop over segment

  for (CSCSegmentCollection::const_iterator segIt_1 = cscSegments->begin(); segIt_1 != cscSegments->end(); segIt_1++) {
    
    CSCDetId id1 = (CSCDetId)(*segIt_1).cscDetId();

    // Restrain to calibrated ME-1/2 chambers
    if (id1.station() != 1 || id1.ring()    != 2 ) continue;  // Look at ME-1/2 chambers only
    if (id1.chamber() < 27 || id1.chamber() > 32 ) continue;  // Look at chambers with calibrations: 27-31

    // Test that have only 1 segment in this chamber  (to avoid combinatorics)
    int NsegPerChamber = 0;
    for (CSCSegmentCollection::const_iterator segIt_3 = cscSegments->begin(); segIt_3 != cscSegments->end(); segIt_3++)
      if ((CSCDetId)(*segIt_3).cscDetId() == id1) NsegPerChamber++;
    if (NsegPerChamber > 1) continue; 
    
    // First segment properties
    LocalVector vec1 = (*segIt_1).localDirection();
    LocalPoint  xyz1 = (*segIt_1).localPosition();
    const CSCChamber* chamber1 = cscGeom->chamber(id1);

    // Test that entrance angle isn't too steep in x-z and y-z plane
    float z1 = vec1.z();
    if (z1 == 0.) z1 = 0.001;
    if ( fabs(vec1.x()/z1) > maxdxdz || fabs(vec1.y()/z1) > maxdydz ) continue;

    // Test that segment is fully within fiducial volume:
    if ( !isSegInFiducial( chamber1, xyz1, vec1, 25.0 ) ) continue;

  
    // Now, look at 6-hit segment efficiency for 2nd segment    
    refMapRaw[id1.chamber()]++;                                   // Count # of events with 6 layers with hits (denominator)
    if ((*segIt_1).nRecHits() == 6 ) segMapRaw[id1.chamber()]++;  // Count # of 6 hit segment (numerator)


    // Look at hit and segment reconstruction efficiency:
    
    // denominators
    segCount[0]++;
    int id = 1;
    for (int chIdx = 27; chIdx < 33; chIdx++) {
      if (id1.chamber() == chIdx) segCount[id]++;
      id++;
    } 
    if ((*segIt_1).nRecHits()==5) segCount5++;

    int nLayers = 0;
    int old_layer = 0;    
    for (CSCRecHit2DCollection::const_iterator recIt = recHits->begin(); recIt != recHits->end(); recIt++) {
      
      // Find chamber with rechits in CSC 
      CSCDetId idrec = (CSCDetId)(*recIt).cscDetId();
      
      if ((idrec.endcap() == id1.endcap()) &&
          (idrec.ring() == id1.ring()) &&
          (idrec.station() == id1.station()) &&
          (idrec.chamber() == id1.chamber())) {
        if (idrec.layer() != old_layer) {
          layMap[0][idrec.layer()]++;
          int id = 1;
          nLayers++;
          for (int chIdx = 27; chIdx < 33; chIdx++) {
            if (id1.chamber() == chIdx) layMap[id][idrec.layer()]++;
            id++;
          }
          if ((*segIt_1).nRecHits()==5) layMap5[idrec.layer()]++;
        }
        old_layer = idrec.layer();
      }
    }


    // Now, look at 6-hit segment efficiency provided 6 rechits...
    if (nLayers == 6) {
      refMap[id1.chamber()]++;                                   // Count # of events with 6 layers with hits (denominator)
      if ((*segIt_1).nRecHits() == 6 ) segMap[id1.chamber()]++;  // Count # of 6 hit segment (numerator)

      if ( noHitNearBoundaries(chamber1, xyz1, vec1) ) {
        refMapCFEB[id1.chamber()]++;                                   // Count # of events with 6 layers with hits (denominator)
        if ((*segIt_1).nRecHits() == 6 ) segMapCFEB[id1.chamber()]++;  // Count # of 6 hit segment (numerator)
      }   
    }

      
    // Cut on the # of hits for overlap studies:    
    if ((*segIt_1).nRecHits() < minnhits) continue;    
    
    
    // Second loop over segment to find matching pair

    for (CSCSegmentCollection::const_iterator segIt_2 = cscSegments->begin(); segIt_2 != cscSegments->end(); segIt_2++) {
      
      if ((*segIt_2).nRecHits() < minnhits2) continue;
      
      CSCDetId id2 = (CSCDetId)(*segIt_2).cscDetId();
          
      // Check that have chambers in same station/ring
      if (id1.station()   == id2.station() &&
          id1.ring()      == id2.ring()   ) {
	// Are stations next to each other ?
        if (id1.chamber() != id2.chamber()-1) continue;  // Order is important as we don't want to duplicate results
      } else {
	continue;
      }
      

      // Test that have only 1 segment in this chamber  (to avoid combinatorics)
      NsegPerChamber = 0;
      for (CSCSegmentCollection::const_iterator segIt_3 = cscSegments->begin(); segIt_3 != cscSegments->end(); segIt_3++)
        if ((CSCDetId)(*segIt_3).cscDetId() == id1) NsegPerChamber++;
      if (NsegPerChamber > 1) continue;
      
      
      
      // Have potentially segments within overlapping region...  
      
      if (debug) cout << "Found potential track pair overlapping" << endl;
      
      // ...extract properties of segments to find out
      
      // Second segment properties
      LocalVector vec2 = (*segIt_2).localDirection();
      LocalPoint  xyz2 = (*segIt_2).localPosition();

      const CSCChamber* chamber2 = cscGeom->chamber(id2);
     
      // Test that segment is fully within fiducial volume:
      if ( !isSegInFiducial( chamber2, xyz2, vec2, 25.0 ) ) continue;
   
          
      // Test if entrance angle is reasonable
      float z2 = vec2.z();
      if (z2 == 0.) z2 = 0.001;
      if ( fabs(vec2.x()/z2) > maxdxdz || fabs(vec2.y()/z2) > maxdydz  ) continue;      
      if (debug) cout << "2nd track satisfy entrance angle criteria" << endl;

      
      // Need to tranform from local to global coordinates:
      
      // First segment
      GlobalPoint  Gxyz1 = chamber1->toGlobal( xyz1 );
      GlobalVector Gvec1 = chamber1->toGlobal( vec1 );
      
      // Second segment
      GlobalPoint  Gxyz2 = chamber2->toGlobal( xyz2 );
      GlobalVector Gvec2 = chamber2->toGlobal( vec2 );
      
      // Spacing between two origins in global coordinates:
      float deltaZ = Gxyz2.z() - Gxyz1.z();
      
      // Compute extrapolated position from 1
      
      // Note that there is confusion about the direction of the vector, so try either sign and
      // test which hypothesis (+/-) is better
      GlobalPoint Gxyz2prime; 
      GlobalPoint Gxyz2primeA(fabs(Gvec1.x()/Gvec1.z()) * deltaZ + Gxyz1.x(), 
		              fabs(Gvec1.y()/Gvec1.z()) * deltaZ + Gxyz1.y(), 
		              Gxyz2.z());
      
      GlobalPoint Gxyz2primeB(-fabs(Gvec1.x()/Gvec1.z()) * deltaZ + Gxyz1.x(), 
			      -fabs(Gvec1.y()/Gvec1.z()) * deltaZ + Gxyz1.y(), 
			      Gxyz2.z());
      
      GlobalVector R2A = Gxyz2 - Gxyz2primeA;
      float dR2A       = R2A.mag();
      GlobalVector R2B = Gxyz2 - Gxyz2primeB;
      float dR2B       = R2B.mag();
      
      Gxyz2prime = Gxyz2primeA;
      if (dR2B < dR2A) Gxyz2prime = Gxyz2primeB;
      
      // Compute extrapolated position from 2
      GlobalPoint Gxyz1prime; 
      GlobalPoint Gxyz1primeA(fabs(Gvec2.x()/Gvec2.z()) * deltaZ + Gxyz2.x(),
			      fabs(Gvec2.y()/Gvec2.z()) * deltaZ + Gxyz2.y(),
			      Gxyz1.z());
      
      GlobalPoint Gxyz1primeB(-fabs(Gvec2.x()/Gvec2.z()) * deltaZ + Gxyz2.x(),
			      -fabs(Gvec2.y()/Gvec2.z()) * deltaZ + Gxyz2.y(),
			      Gxyz1.z());

      GlobalVector R1A = Gxyz1 - Gxyz1primeA;
      float dR1A       = R1A.mag();
      GlobalVector R1B = Gxyz1 - Gxyz1primeB;
      float dR1B       = R1B.mag();

      Gxyz1prime = Gxyz1primeA;
      if (dR1B < dR1A) Gxyz1prime = Gxyz1primeB;


      // Transform the above into local coordinates of chambers:
      LocalPoint xyz1prime = chamber1->toLocal( Gxyz1prime );
      LocalPoint xyz2prime = chamber2->toLocal( Gxyz2prime );


      // Compare origin of segment 1 with predicated from segment 2:
      float dphi1     = Gxyz1.phi() - Gxyz1prime.phi();
      GlobalVector R1 = Gxyz1 - Gxyz1prime;
      float dR1       = R1.mag(); 
      float dx1       = xyz1.x() - xyz1prime.x();
      float dy1       = xyz1.y() - xyz1prime.y();

      // Compare origin of segment 2 with predicated from segment 1:
      float dphi2     = Gxyz2.phi() - Gxyz2prime.phi();
      GlobalVector R2 = Gxyz2 - Gxyz2prime;
      float dR2       = R2.mag(); 
      float dx2       = xyz2.x() - xyz2prime.x();
      float dy2       = xyz2.y() - xyz2prime.y();

      // Find angular difference between two segments.
      // Use dot product:  cos(theta_12) = v1 . v2 / [ |v1|*|v2| ]

      float costheta12 = Gvec1.dot(Gvec2)/( Gvec1.mag() * Gvec2.mag() );

      // By definition, there is no correct +/- direction, so set to + always
      if (costheta12 < 0.) costheta12 = -costheta12;

      // Further selection requirements:  
      // 1) Delta phi must be reasonable (how many strips away)
      // 2) Require segment vectors to point reasonably towards the same direction (solid angle)
      // 3) Require Delta R to be N wire groups away max = X cm

      if ( dphi1 > maxDphi || dphi2 > maxDphi ) continue;
      if (costheta12 < minCosTheta12) continue;
      if (dR1 > maxDR || dR2 > maxDR ) continue;

      if (debug) cout << "*** Segment pair satisfies selection " << endl;


      // Look at rechit efficiency for the overlapping chambers

      // denominator for rechit efficiency
      OversegCount++;
      if ((*segIt_2).nRecHits()==5) OversegCount5++;  // 5-hit segment case

      old_layer = 0;
      for (CSCRecHit2DCollection::const_iterator recIt = recHits->begin(); recIt != recHits->end(); recIt++) {
  
        // Find chamber with rechits in CSC
        CSCDetId idrec = (CSCDetId)(*recIt).cscDetId();
  
        if ((idrec.endcap() == id2.endcap()) &&
            (idrec.ring() == id2.ring()) &&
            (idrec.station() == id2.station()) &&
            (idrec.chamber() == id2.chamber())) {
          if (idrec.layer() != old_layer) {
            OverlayMap[idrec.layer()]++;
            if ((*segIt_2).nRecHits()==5) OverlayMap5[idrec.layer()]++;
          }
          old_layer = idrec.layer();
        }
      }
     

      // Generate histograms
      Histos *histo = 0;
      if (id1.chamber() == 27) {
	histo = h1;
      } else if (id1.chamber() == 28) {
	histo = h2;
      } else if (id1.chamber() == 29) {
	histo = h3;
      } else if (id1.chamber() == 30) {
	histo = h4;
      } else if (id1.chamber() == 31) {
	histo = h5;
      } else {
	continue;   // wrong chamber !!!
      }
      if (debug) cout << "Have match and will fill histograms for " << Noverlaps << "th segment pair" << endl;	
      if (debug) cout << dphi1 << " " << dx1 << " " << dy1 << " " << dR1 << " " 
                      << dphi2 << " " << dx2 << " " << dy2 << " " << dR2 << " " << costheta12 << endl;

      histo->Fill(dphi1, dx1, dy1, dR1, dphi2, dx2, dy2, dR2, costheta12);

      Noverlaps++;
    }
  }
}


bool CSCMTCCOverlap::noHitNearBoundaries( const CSCChamber* chamber, LocalPoint lp, LocalVector vec ) { 


  float dxdz = vec.x()/vec.z();
  float dydz = vec.y()/vec.z();

  GlobalPoint gp = chamber->toGlobal( lp );

  for ( int lay = 1; lay < 7; lay++) {
    const CSCLayer* layer = chamber->layer(lay);
    const CSCLayerGeometry* layergeom = layer->geometry();

    // Find the "z" at layer:
    LocalPoint lpl = layer->toLocal( gp);
    float zl = lpl.z(); 

    // Propagate segment in local coordinates wrt center of chamber
    float dz = zl - lp.z(); 
    float xl = dxdz * dz + lp.x();
    float yl = dydz * dz + lp.y();

    // Bring to global coordinates, and then to layer coordinates
    LocalPoint lp2(xl,yl,zl);
    GlobalPoint gp2 = chamber->toGlobal( lp2 );
    LocalPoint lp3 = layer->toLocal( gp2 );
    
    int nearestStrip = layergeom->nearestStrip( lp3 );

    if (nearestStrip%16 < 2 ) return false;

  }

  return true;
}


bool CSCMTCCOverlap::isSegInFiducial( const CSCChamber* chamber, LocalPoint lp, LocalVector vec, float ChamberThickness ) { 

  // Margin around chambers to ensure hit is well within fiducial volume
  float marginAtEdges = ChamberMarginAtEdges; // now read in from config file

  float dz = ChamberThickness/2.;

  const CSCLayer* layer1 = chamber->layer(1);
  const CSCLayerGeometry* layergeom = layer1->geometry();
  float apothem     = layergeom->length()/2.;
  float widthTop    = layergeom->width()/2.;                     // Note these are half-widths
  float widthBottom = layergeom->widthAtHalfLength() - widthTop; // t+b=2w
  float slopeSide   = (widthTop - widthBottom) / (2. * apothem);
  float interSide   = slopeSide * layergeom->widthAtHalfLength()/2.;
  interSide = fabs(interSide);

  float dxdz = vec.x()/vec.z();
  float dydz = vec.y()/vec.z();
   
  float XatZ_pos = dxdz * dz + lp.x();
  float YatZ_pos = dydz * dz + lp.y();
  float XatZ_neg =-dxdz * dz + lp.x();
  float YatZ_neg =-dydz * dz + lp.y();

  // First look at top / bottom 
  if (fabs(YatZ_neg) > apothem -marginAtEdges || fabs(YatZ_pos) > apothem - marginAtEdges) return false;

  // Now look at sides and use left-right chamber symmetry to ease test
  float maxX_pos = fabs((YatZ_pos + interSide)/slopeSide) - marginAtEdges;
  float maxX_neg = fabs((YatZ_neg + interSide)/slopeSide) - marginAtEdges;

  if (fabs(XatZ_pos) > maxX_pos || fabs(XatZ_neg) > maxX_neg) return false;

  return true;

}

DEFINE_FWK_MODULE(CSCMTCCOverlap)
