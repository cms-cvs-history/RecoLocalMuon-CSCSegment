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


  if (debug) cout << "[CSCMTCCOverlap] Constructor called" << endl;
 
  // Create the root file
  theFile = new TFile(rootFileName.c_str(), "RECREATE");

  // Book the histograms
  h1  = new Histos("ME12_27_28");
  h2  = new Histos("ME12_28_29");
  h3  = new Histos("ME12_29_30");
  h4  = new Histos("ME12_30_31"); 
  
  Noverlaps = 0;
}


// Destructor
CSCMTCCOverlap::~CSCMTCCOverlap(){

  hlayeff = new TH1F("hlayeff", "rechit eff per layer", layMap.size()*2 + 2, 0, layMap.size()*2 + 2); 
  hsegeff = new TH1F("hsegeff", "6-hit segment eff", chaMap.size()*2 + 2, 0, chaMap.size()*2 + 2); 

  int ibin = 0;
  cout << "Rechit efficiency for building hit on segment/layer" << endl;        
  for (map<int,int>::const_iterator it = layMap.begin(); it != layMap.end(); it++) {
    ibin++;
    float eff = (float)it->second/(float)refMap2[it->first]; 
    hlayeff->SetBinContent(ibin*2, eff);
//    hlayeff->GetXaxis()->SetBinLabel(ibin*2, string(it->first));
    cout << "Layer" << it->first << "  : " << it->second << " " << refMap2[it->first] 
                                         << "  "       << eff << endl;
  }
  ibin = 0;
  cout << "Reco efficiency for building 6-hit segment" << endl;        
  for (map<int,int>::const_iterator it = chaMap.begin(); it != chaMap.end(); it++) {
    ibin++;
    float eff = (float)it->second/(float)refMap[it->first]; 
    hsegeff->SetBinContent(ibin*2, eff);
//    hsegeff->GetXaxis()->SetBinLabel(ibin*2, string(it->first));
    cout << "Chamber" << it->first << ": " << it->second << " " << refMap[it->first] 
                                           << "  "       << eff << endl;
  }


  theFile->cd();
  hlayeff->Write();
  hsegeff->Write();
  h1->Write();
  h2->Write();
  h3->Write();
  h4->Write();

  theFile->Close();
  if (debug) cout << "[CSCMTCCOverlap] Finished writing histograms to file" << endl;

  cout << "[CSCMTCCOverlap] Number of segment pairs for overlapping region is " << Noverlaps << endl;

}


// The main
void CSCMTCCOverlap::analyze(const Event & event, const EventSetup& eventSetup){
   
  // Get the CSC Geometry :
  ESHandle<CSCGeometry> cscGeom;
  eventSetup.get<MuonGeometryRecord>().get(cscGeom);
 
  // Get the Segments collection :
  Handle<CSCSegmentCollection> cscSegments; 
  event.getByLabel(cscSegmentLabel, cscSegments);  
  if (debug) cout << " " << endl;
  if (debug) cout << "   #cscSegments: " << cscSegments->size() << endl;

  // Get the RecHits collection :
  Handle<CSCRecHit2DCollection> recHits; 
  event.getByLabel(recHitLabel, recHits);  
  if (debug) cout << "   #RecHits: " << recHits->size() << endl;
  if (debug) cout << " " << endl;

  // First loop over segment
  for (CSCSegmentCollection::const_iterator segIt_1 = cscSegments->begin(); segIt_1 != cscSegments->end(); segIt_1++) {

    if ((*segIt_1).nRecHits() < minnhits) continue;
    
    CSCDetId id1 = (CSCDetId)(*segIt_1).cscDetId();
    if (id1.station() != 1 || id1.ring()    != 2 ) continue;  // Look at ME-1/2 chambers only
    if (id1.chamber() < 26 || id1.chamber() > 30 ) continue;  // Look at chambers with calibrations: 27-31


    // Second loop over segment to find matching pair
    for (CSCSegmentCollection::const_iterator segIt_2 = cscSegments->begin(); segIt_2 != cscSegments->end(); segIt_2++) {
   
      if ((*segIt_2).nRecHits() < minnhits2) continue;

      CSCDetId id2 = (CSCDetId)(*segIt_2).cscDetId();


      // Test that have only 1 segment in this chamber  (to avoid combinatorics)
      int NsegPerChamber = 0;
      for (CSCSegmentCollection::const_iterator segIt_3 = cscSegments->begin(); segIt_3 != cscSegments->end(); segIt_3++) {
        CSCDetId id3 = (CSCDetId)(*segIt_3).cscDetId();
        if (id3 == id2) NsegPerChamber++;
      }     
      if (NsegPerChamber > 1) continue;
     
      
      // Check that have chambers in same station/ring
      if (id1.station()   == id2.station() &&
          id1.ring()      == id2.ring()   ) {
	// Are stations next to each other ?
        if (id1.chamber() != id2.chamber()-1) continue;  // Order is important as we don't want to duplicate results
      } else {
	continue;
      }


      // Have potentially segments within overlapping region...  

      if (debug) cout << "Found potential track pair overlapping" << endl;

      // ...extract properties of segments to find out

      // First segment properties
      LocalVector vec1 = (*segIt_1).localDirection();
      LocalPoint  xyz1 = (*segIt_1).localPosition();
    
      // Second segment properties
      LocalVector vec2 = (*segIt_2).localDirection();
      LocalPoint  xyz2 = (*segIt_2).localPosition();


      // Test if entrance angle for either segments is reasonable
      float z1 = vec1.z();
      float z2 = vec2.z();
      if (z1 == 0.) z1 = 0.001;
      if (z2 == 0.) z2 = 0.001;
      if (fabs(vec1.x()/z1) > maxdxdz || fabs(vec2.x()/z2) > maxdxdz || 
          fabs(vec1.y()/z1) > maxdydz || fabs(vec2.y()/z2) > maxdydz  ) continue;

      if (debug) cout << "tracks satisfy entrance angle criteria" << endl;

      // Need to tranform from local to global coordinates:

      // First segment
      const CSCChamber* ch1 = cscGeom->chamber( id1 );
      GlobalPoint  Gxyz1 = ch1->toGlobal( xyz1 );
      GlobalVector Gvec1 = ch1->toGlobal( vec1 );
      
      // Second segment
      const CSCChamber* ch2 = cscGeom->chamber( id2 );
      GlobalPoint  Gxyz2 = ch2->toGlobal( xyz2 );
      GlobalVector Gvec2 = ch2->toGlobal( vec2 );

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
      LocalPoint xyz1prime = ch1->toLocal( Gxyz1prime );
      LocalPoint xyz2prime = ch2->toLocal( Gxyz2prime );


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


      // Now, look at 6-hit segment efficiency for 2nd segment    
      refMap[id2.chamber()]++;    // Reference (denominator)
      if ((*segIt_2).nRecHits() == 6) chaMap[id2.chamber()]++;  // Count # of 6 hit segment

      // and rechit efficiency per layer for 2nd segment    
      for (int k = 1; k < 7; k++) refMap2[k]++;  // Reference (denominator)

      const std::vector<CSCRecHit2D>& rh = (*segIt_1).specificRecHits();
      for (std::vector<CSCRecHit2D>::const_iterator rhIter = rh.begin(); rhIter!=rh.end(); ++rhIter) { 
        CSCDetId idrh = (CSCDetId)(*rhIter).cscDetId();
        layMap[idrh.layer()]++;
      }

     
      // Generate histograms
      Histos *histo = 0;
      if (id1.chamber() == 27) histo = h1;
      if (id1.chamber() == 28) histo = h2;
      if (id1.chamber() == 29) histo = h3;
      if (id1.chamber() == 30) histo = h4;
      if (debug) cout << "Have match and will fill histograms !" << endl;	

      histo->Fill(dphi1, dx1, dy1, dR1, dphi2, dx2, dy2, dR2, costheta12);
      Noverlaps++;
    }
  }
}



bool CSCMTCCOverlap::isSegInFiducial( const CSCChamber* chamber, LocalPoint lp, LocalVector vec, float ChamberThickness ) { 

  // Margin around dectector to ensure hit is well within fiducial volume
  float marginAtEdges = 2.; 

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