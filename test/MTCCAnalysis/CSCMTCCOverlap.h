#ifndef CSCSegment_CSCMTCCOverlap_H
#define CSCSegment_CSCMTCCOverlap_H

/** \class CSCMTCCOverlap
 *  Basic analyzer class which accesses segments
 *  and studies the overlap region properties: 
 *  segment matching, efficiency...
 *
 *  Author: D. Fortin - UC Riverside
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Handle.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>

#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalVector.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include <Geometry/CSCGeometry/interface/CSCChamber.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include "FWCore/Framework/interface/ESHandle.h"
#include "CSCMTCCOverlapHisto.h"

#include <vector>
#include <map>
#include <string>

#include "TFile.h"
#include "TH1F.h"

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

class TFile;
class CSCLayer;
class CSCDetId;

class CSCMTCCOverlap : public edm::EDAnalyzer {
public:

  /// Constructor
  CSCMTCCOverlap(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~CSCMTCCOverlap();

  /// Perform the real analysis
  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

  bool isSegInFiducial( const CSCChamber* chamber, LocalPoint lp, LocalVector vec, float ChamberThickness );
  bool noHitNearBoundaries( const CSCChamber* chamber, LocalPoint lp, LocalVector vec );

private: 

  // Counter
  int Noverlaps;

  // Histograms stuff;
  TFile *theFile;
  TH1F *hlayeff[7], *hlayeff5, *hOverlayeff, *hOverlayeff5, *hsegeffRaw, *hsegeff, *hsegeffCFEB;
  Histos* h1;
  Histos* h2;
  Histos* h3;
  Histos* h4;
  Histos* h5;


  // Input from cfg file
  bool  debug;
  float maxdxdz;
  float maxdydz;
  int   minnhits;
  int   minnhits2;
  float minCosTheta12;
  float maxDphi;
  float maxDR;
  float ChamberMarginAtEdges;
  std::string rootFileName;
  std::string cscSegmentLabel;
  std::string recHitLabel;

  // For hit efficiency
  long segCount[7], segCount5, OversegCount, OversegCount5;
  std::map<int, int> layMap[7];
  std::map<int, int> layMap5;
  std::map<int, int> OverlayMap;
  std::map<int, int> OverlayMap5;
  // For segment efficiencies
  std::map<int, int> refMapRaw;
  std::map<int, int> segMapRaw;
  std::map<int, int> refMap;
  std::map<int, int> segMap;
  std::map<int, int> refMapCFEB;
  std::map<int, int> segMapCFEB;

};


#endif

