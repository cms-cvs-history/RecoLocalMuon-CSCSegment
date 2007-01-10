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

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include <Geometry/CSCGeometry/interface/CSCChamber.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include "FWCore/Framework/interface/ESHandle.h"
#include "CSCMTCCOverlapHisto.h"

#include <vector>
#include <map>
#include <string>

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


private: 

  // Counter
  int Noverlaps;

  // Histograms;
  Histos* h1;
  Histos* h2;
  Histos* h3;
  Histos* h4;


  // Input from cfg file
  bool  debug;
  float maxdxdz;
  float maxdydz;
  int   minnhits;
  float minCosTheta12;
  float maxDphi;
  std::string rootFileName;
  std::string cscSegmentLabel;
  std::string recHitLabel;

  TFile *theFile;

};


#endif

