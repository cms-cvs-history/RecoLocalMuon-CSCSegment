#ifndef CSCSegment_CSCMTCCAnalysis_H
#define CSCSegment_CSCMTCCAnalysis_H

/** \class CSCMTCCAnalysis
 *  Basic analyzer class which accesses segments
 *  and studies the properties: residuals, pulls, etc...
 *
 *  Author: D. Fortin  - UC Riverside
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2D.h"
#include "DataFormats/CSCRecHit/interface/CSCSegment.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"

#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCChamber.h"
#include "Geometry/CSCGeometry/interface/CSCLayer.h"

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
  void SKFitSlope(edm::ESHandle<CSCGeometry> cscGeom, int rechit_size, std::vector<CSCRecHit2D> rh_V00);
  void x_pull(const CSCSegment* seg, edm::ESHandle<CSCGeometry> cscGeom);
  void residual(const CSCSegment* seg, edm::ESHandle<CSCGeometry> cscGeom);
  void hit_info(LocalPoint rh_p, CSCDetId det_id, edm::ESHandle<CSCGeometry> cscGeom);
  void rotation(const CSCSegment* seg, LocalPoint fit_p[6], edm::ESHandle<CSCGeometry> cscGeom);
  void segment_chamber(CSCDetId det_id);

  // Histograms
  H2DRecHit *hRHPME[9];
  H2DRecHit1 *hChamber1[9];
  H2DRecHit1 *hChamber2[30];
//  H2DRecHit1 *hRHPMEa;
  H2DRecHit2 *hRHPMEb;
  // The file which will store the histos
  TFile *theFile;


  // event selection variables
  double maxdxdz,maxdydz;
  int x_margin_L;
  int x_margin_R;
  double y_margin_T;
  double y_margin_B;
  bool CFEB_edge;

  // x pull outputs
  double pull[6];

  // residual outputs
  /// x,y,Rdphi,Rdphi(strip width)
  double resid_X[6];
  double resid_Y[6];
  double resid_Rdf[6];
  double resid_RdfWth[6];
  /// fitting point, fitting point detID
  LocalPoint fhit_cont[6];
  CSCDetId det_fit[6];
  /// skip rechit information
  LocalPoint rhit_cont[6];
  double xRecErr[6];
  double xFitErr[6];
  /// wire_group
  int wire_group[6];

  // hit info
  bool rh_type_i;
  bool rh_side_i;
  double strip_x; 

  // rotation
  double rh_u[6];
  double fit_u[6];

  // segment per chamber
  int nu_seg1;
  bool seg_count1;
  int DetNu[4];

  // public container
  double cont[6];

  // SK Fitting variables
  double SKFit[4];
  double SKErr[6];
  //RecHitsContainer rh_V0;
   std::vector<CSCRecHit2D> rh_V0;
   std::vector<CSCRecHit2D> rh_Vec;

  // Switch for debug output
  bool debug;
  // Root file name
  std::string rootFileName;
  std::string cscSegmentLabel;
  std::string recHitLabel;

};


#endif

