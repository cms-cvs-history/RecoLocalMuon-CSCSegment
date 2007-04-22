#ifndef CSCSegment_CSCSegAlgoST_h
#define CSCSegment_CSCSegAlgoST_h

/**
 * \class CSCSegAlgoST
 *
 * This algorithm is based on the Minimum Spanning Tree (ST) approach 
 * for building endcap muon track segments out of the rechit's in a CSCChamber.<BR>
 *
 * A CSCSegment is a RecSegment4D, and is built from
 * CSCRecHit2D objects, each of which is a RecHit2DLocalPos. <BR>
 *
 * This builds segments consisting of at least 3 hits. It is allowed for segments to have 
 * a common (only one) rechit.  
 * 
 * The program is under construction/testing.
 *
 *  \authors S. Stoynev - NU
 *           I. Bloch   - FNAL
 *           E. James   - FNAL
 *
 */

#include <RecoLocalMuon/CSCSegment/src/CSCSegmentAlgorithm.h>

#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>

#include <deque>
#include <vector>

class CSCSegAlgoST : public CSCSegmentAlgorithm {

public:

  /// Typedefs
    
  typedef std::vector<const CSCRecHit2D*> ChamberHitContainer;
  typedef std::vector < std::vector<const CSCRecHit2D* > > Segments;
  typedef std::deque<bool> BoolContainer;
    
  /// Constructor
  explicit CSCSegAlgoST(const edm::ParameterSet& ps);

  /// Destructor
  virtual ~CSCSegAlgoST() {};

  /**
   * Build track segments in this chamber (this is where the actual
   * segment-building algorithm hides.)
   */
  std::vector<CSCSegment> buildSegments(ChamberHitContainer rechits);

  /**
   * Build track segments in this chamber (this is where the actual
   * segment-building algorithm hides.)
   */
  std::vector<CSCSegment> buildSegments2(ChamberHitContainer rechits);

  /**
   * Here we must implement the algorithm
   */
  std::vector<CSCSegment> run(const CSCChamber* aChamber, ChamberHitContainer rechits); 

  /**
   * Build groups of rechits that are separated in x and y to save time on the segment finding
   */
  std::vector< std::vector<const CSCRecHit2D*> > clusterHits(const CSCChamber* aChamber, ChamberHitContainer rechits);

  /**
   * Remove bad hits from found segments based not only on chi2, but also on charge and 
   * further "low level" chamber information.
   */
  std::vector< CSCSegment > prune_bad_hits(const CSCChamber* aChamber, std::vector< CSCSegment > segments);

private:

  /// Utility functions 
  double theWeight(double coordinate_1, double coordinate_2, double coordinate_3);

  void ChooseSegments(void);

  // siplistic routine - just return the segment with the smallest weight
  void ChooseSegments2a(int best_seg);

  // copy of Stoyans ChooseSegments adjusted to the case without fake hits
  void ChooseSegments2(int best_seg);

  // Choose routine with reduce nr of loops
  void ChooseSegments3(int best_seg);

  //
  void fitSlopes(void);
  void fillChiSquared(void);
  void fillLocalDirection(void);
  HepMatrix derivativeMatrix(void) const;
  AlgebraicSymMatrix weightMatrix(void) const;
  AlgebraicSymMatrix calculateError(void) const;
  void flipErrors(AlgebraicSymMatrix&) const;
  // Member variables
  const std::string myName; 
  const CSCChamber* theChamber;
  Segments GoodSegments;

  ChamberHitContainer PAhits_onLayer[6];
  ChamberHitContainer Psegments_hits;

  std::vector< ChamberHitContainer > Psegments;
  std::vector< float > weight_A;

  //ibl

  ChamberHitContainer protoSegment;
  float       protoSlope_u;
  float       protoSlope_v;
  LocalPoint  protoIntercept;		
  double      protoChi2;
  LocalVector protoDirection;

  // input from .cfi file
  bool    debug;
  //  int     minLayersApart;
  //  double  nSigmaFromSegment;
  int     minHitsPerSegment;
  //  int     muonsPerChamberMax;
  //  double  chi2Max;
  double  dXclusBoxMax;
  double  dYclusBoxMax;
  int     maxRecHitsInCluster;
  bool    preClustering;
  bool    Pruning;
  bool    BrutePruning;
  bool    onlyBestSegment;
};

#endif