#ifndef CSCSegment_CSCMTCCAnalysis_H
#define CSCSegment_CSCMTCCAnalysis_H

/** \class CSCMTCCAnalysis
 *  Basic analyzer class which accesses segments
 *  and studies the properties: residuals, pulls, etc...
 *
 *  Author: D. Fortin  - UC Riverside
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Handle.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include <Geometry/CSCGeometry/interface/CSCChamber.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include "FWCore/Framework/interface/ESHandle.h"
#include "CSCMTCCHistograms.h"

#include <vector>
#include <map>
#include <string>

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

//class PSimHit;
class TFile;
class CSCLayer;
class CSCDetId;

class CSCMTCCAnalysis : public edm::EDAnalyzer {
public:

//  typedef std::vector<CSCRecHit2D> RecHitsContainer;
//  typedef std::vector<CSCRecHit2D>::const_iterator RecHitsContainerIt;


  /// Constructor
  CSCMTCCAnalysis(const edm::ParameterSet& pset);

  /// Destructor
  virtual ~CSCMTCCAnalysis();

  // Operations

  /// Perform the real analysis
  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);


protected:

private: 

  // Utility functions
  //void SKFitSlope(const edm::EventSetup& eventSetup);
  void SKFitSlope(edm::ESHandle<CSCGeometry> cscGeom, int rechit_size);

  // Histograms
  H2DRecHit *hRHPME[9];
  H2DRecHit1 *hRHPMEa;
//  H2DRecHit2 *hRHPMEb;
  H2DRecHit3 *hChamber1[9];
  H2DRecHit3 *hChamber2[30];

  // The file which will store the histos
  TFile *theFile;

  // Switch for debug output
  bool debug;

  // event selection variables
  float maxdxdz,maxdydz;
  int x_margin_L;
  int x_margin_R;
  double y_margin_T;
  double y_margin_B;

  // SK Fitting variables
  double SKFit[4];
  double SKErr[6];
  //RecHitsContainer rh_V0;
   std::vector<CSCRecHit2D> rh_V0;

  // Root file name
  std::string rootFileName;
  std::string cscSegmentLabel;
  std::string recHitLabel;

};


#endif

