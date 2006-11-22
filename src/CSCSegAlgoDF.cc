/**
 * \file CSCSegAlgoDF.cc
 *
 *  \author Dominique Fortin -UCR
 */
 
#include <RecoLocalMuon/CSCSegment/src/CSCSegAlgoDF.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/Vector/interface/GlobalPoint.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>


/* Constructor
 *
 */
CSCSegAlgoDF::CSCSegAlgoDF(const edm::ParameterSet& ps) : CSCSegmentAlgorithm(ps), myName("CSCSegAlgoDF") {
	
  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");
  minLayersApart         = ps.getUntrackedParameter<int>("minLayersApart");
  nSigmaFromSegment      = ps.getUntrackedParameter<double>("nSigmaFromSegment");
  minHitsPerSegment      = ps.getUntrackedParameter<int>("minHitsPerSegment");
  muonsPerChamberMax     = ps.getUntrackedParameter<int>("CSCSegmentPerChamberMax");      
  chi2Max                = ps.getUntrackedParameter<double>("chi2Max");
	
}


std::vector<CSCSegment> CSCSegAlgoDF::run(const CSCChamber* aChamber, ChamberHitContainer rechits) {

  // Store chamber in temp memory
  theChamber = aChamber; 

  return buildSegments(rechits); 
}


/* This builds segments by first creating proto-segments from at least 3 hits.
 * We intend to try all possible pairs of hits to start segment building. 'All possible' 
 * means each hit lies on different layers in the chamber.  Once a hit has been assigned 
 * to a segment, we don't consider it again, THAT IS, FOR THE FIRST PASS ONLY !
 * In fact, this is one of the possible flaw with the SK algorithms as it sometimes manages
 * to build segments with the wrong starting points.  In the DF algorithm, the endpoints
 * are tested as the best starting points in a 2nd and 3rd loop.
 *
 * Also, only a certain muonsPerChamberMax maximum number of segments can be produced in the chamber
 */
std::vector<CSCSegment> CSCSegAlgoDF::buildSegments(ChamberHitContainer rechits) {

  // Clear buffer for segment vector
  std::vector<CSCSegment> segmentInChamber;
  segmentInChamber.clear();

  if (rechits.size() < 2) return segmentInChamber;

  LayerIndex layerIndex( rechits.size() );
  
  for ( unsigned int i = 0; i < rechits.size(); i++ ) {
    
    layerIndex[i] = rechits[i]->cscDetId().layer();
  }
  
  double z1 = theChamber->layer(1)->position().z();
  double z6 = theChamber->layer(6)->position().z();
  
  if ( z1 > 0. ) {
    if ( z1 > z6 ) { 
      reverse( layerIndex.begin(), layerIndex.end() );
      reverse( rechits.begin(),    rechits.end() );
    }    
  }
  else if ( z1 < 0. ) {
    if ( z1 < z6 ) {
      reverse( layerIndex.begin(), layerIndex.end() );
      reverse( rechits.begin(),    rechits.end() );
    }    
  }


  // Initialize flags that a given hit has been allocated to a segment
  for (unsigned i = 0; i < rechits.size(); i++) usedHits[i] = false ;
  
  ChamberHitContainerCIt ib = rechits.begin();
  ChamberHitContainerCIt ie = rechits.end();

      
  // Allow to have at maximum muonsPerChamberMax muons tracks in a given chamber...
  for ( int pass = 0; pass < muonsPerChamberMax; pass++) {    
   
    
    // Now Loop over hits within the chamber to find 1st seed for segment building
    for ( ChamberHitContainerCIt i1 = ib; i1 < ie; ++i1 ) {
      if ( usedHits[i1-ib] ) continue;

      bool segok = false;
      
      const CSCRecHit2D* h1 = *i1;
      int layer1 = layerIndex[i1-ib];
           
      // Loop over hits backward to find 2nd seed for segment building
      for ( ChamberHitContainerCIt i2 = ie-1; i2 > ib; --i2 ) {	

        // Clear proto segment so it can be (re)-filled 
	protoSegment.clear();

	if ( usedHits[i2-ib] ) continue;   // Hit has been used already

        const CSCRecHit2D* h2 = *i2;	
        int layer2 = layerIndex[i2-ib];
	
	if ( (layer2 - layer1) < minLayersApart ) continue;
	
	if ( !addHit(h1, layer1) ) continue;
	if ( !addHit(h2, layer2) ) continue;
	
	// Try adding hits to proto segment
	tryAddingHitsToSegment(rechits, i1, i2); 
	
	// Check no. of hits on segment, and if enough flag them as used
	segok = isSegmentGood(rechits);
	if ( segok ) {
          if ( debug ) std::cout << "Found a segment !!!" << std::endl;

          // Flag used hits
	  flagHitsAsUsed(rechits);

          // calculate error matrix
          AlgebraicSymMatrix protoErrors = calculateError();     

          CSCSegment temp(protoSegment, protoIntercept, protoDirection, protoErrors, protoChi2); 
              
          segmentInChamber.push_back(temp); 
	  protoSegment.clear();
        }
      } 
    } 
  }
  return segmentInChamber;
}


/* Method tryAddingHitsToSegment
 *
 * Look at left over hits and try to add them to proto segment by looking how far they
 * are from the segment in terms of the hit error matrix (so how many sigmas away).
 *
 */
void CSCSegAlgoDF::tryAddingHitsToSegment( const ChamberHitContainer& rechits, 
                                           const ChamberHitContainerCIt i1, 
                                           const ChamberHitContainerCIt i2) {
  
/* Iterate over the layers with hits in the chamber
 * Skip the layers containing the segment endpoints on first 2 passes, but then
 * try hits on layer containing the segment starting points on 2nd and/or 3rd pass 
 * if segment has >2 hits
 * Test each hit on the other layers to see if it is near the segment using error ellipse
 * on hit.
 * If it is, see whether there is already a hit on the segment from the same layer
 *    - if so, and there are more than 2 hits on the segment, copy the segment,
 *      replace the old hit with the new hit. If the new segment chi2 is better
 *      then replace the original segment with the new one 
 *    - if not, copy the segment, add the hit if it's within a certain range. 
 */  
  
  ChamberHitContainerCIt ib = rechits.begin();
  ChamberHitContainerCIt ie = rechits.end();
  
  for ( int pass = 0; pass < 5; pass++) {
    
    for ( ChamberHitContainerCIt i = ib; i != ie; ++i ) {
      
      if ( usedHits[i-ib] ) continue;   // Don't use hits already part of a segment.
      
      if (pass < 2) if (i == i1 || i == i2 ) continue;  // For first 2 pass, don't try changing endpoints (seeds).

      const CSCRecHit2D* h = *i;      
      int layer = (*i)->cscDetId().layer();
      
      if ( isHitNearSegment( h ) ) {
	if ( hasHitOnLayer(layer) ) {
	  // If segment > 2 hits, try changing endpoints
	  if ( protoSegment.size() > 2 ) {
	    compareProtoSegment(h, layer); 
	  } 
	} else {
	  increaseProtoSegment(h, layer);
	}
      } 
    }
  } 
}


/* Method addHit
 *
 * Test if can add hit to proto segment. If so, try to add it.
 *
 */
bool CSCSegAlgoDF::addHit(const CSCRecHit2D* aHit, int layer) {
  
  // Return true if hit was added successfully and then parameters are updated.
  // Return false if there is already a hit on the same layer, or insert failed.
  
  bool ok = true;
  
  // Test that we are not trying to add the same hit again
  for ( ChamberHitContainer::const_iterator it = protoSegment.begin(); it != protoSegment.end(); it++ ) 
    if ( aHit == (*it)  ) ok = false;
  
  if ( ok ) {
    protoSegment.push_back(aHit);
    updateParameters();
  }
  return ok;
}    


/* Method updateParameters
 *      
 * Perform a simple Least Square Fit on proto segment to determine slope and intercept
 *
 */   
void CSCSegAlgoDF::updateParameters() {
  
  //  no. of wire hits in the proto segment
  //  By construction this is the no. of layers with hits
  //  since we allow just one hit per layer in a segment.
  
  int nh = protoSegment.size();
  
  // First hit added to a segment must always fail here
  if ( nh < 2 ) return;
  
  if ( nh == 2 ) {
    
    // Once we have two hits we can calculate a straight line 
    // (or rather, a straight line for each projection in xz and yz.)
    ChamberHitContainer::const_iterator ih = protoSegment.begin();
    int il1 = (*ih)->cscDetId().layer();
    const CSCRecHit2D& h1 = (**ih);
    ++ih;    
    int il2 = (*ih)->cscDetId().layer();
    const CSCRecHit2D& h2 = (**ih);
    
    //@@ Skip if on same layer, but should be impossible
    if (il1 == il2)  return;
    
    const CSCLayer* layer1 = theChamber->layer(il1);
    const CSCLayer* layer2 = theChamber->layer(il2);
    
    GlobalPoint h1glopos = layer1->toGlobal(h1.localPosition());
    GlobalPoint h2glopos = layer2->toGlobal(h2.localPosition());
    
    // localPosition is position of hit wrt layer (so local z = 0)
    protoIntercept = h1.localPosition();
    
    // We want hit wrt chamber (and local z will be != 0)
    LocalPoint h1pos = theChamber->toLocal(h1glopos);  
    LocalPoint h2pos = theChamber->toLocal(h2glopos);  
    
    float dz = h2pos.z()-h1pos.z();
    protoSlope_u = (h2pos.x() - h1pos.x())/dz ;
    protoSlope_v = (h2pos.y() - h1pos.y())/dz ;
    
    protoChi2 = 0.;

  } else if (nh > 2) {
    
    // When we have more than two hits then we can fit projections to straight lines
    fitSlopes();  
    fillChiSquared();
  }

  fillLocalDirection();
}


/* Method fitSlopes
 *
 * Perform a Least Square Fit on proto segment as per SK algo
 *
 */
void CSCSegAlgoDF::fitSlopes() {
  
  HepMatrix M(4,4,0);
  HepVector B(4,0);
  
  ChamberHitContainer::const_iterator ih = protoSegment.begin();
  
  for (ih = protoSegment.begin(); ih != protoSegment.end(); ++ih) {
    
    const CSCRecHit2D& hit = (**ih);
    const CSCLayer* layer  = theChamber->layer(hit.cscDetId().layer());
    GlobalPoint gp         = layer->toGlobal(hit.localPosition());
    LocalPoint  lp         = theChamber->toLocal(gp); 
    
    // ptc: Local position of hit w.r.t. chamber
    double u = lp.x();
    double v = lp.y();
    double z = lp.z();
    
    // ptc: Covariance matrix of local errors 
    HepMatrix IC(2,2);
    IC(1,1) = hit.localPositionError().xx();
    IC(1,2) = hit.localPositionError().xy();
    IC(2,2) = hit.localPositionError().yy();
    IC(2,1) = IC(1,2); // since Cov is symmetric
    
    // ptc: Invert covariance matrix (and trap if it fails!)
    int ierr = 0;
    IC.invert(ierr); // inverts in place
    if (ierr != 0) {
      LogDebug("CSC") << "CSCSegment::fitSlopes: failed to invert covariance matrix=\n" << IC << "\n";      
    }
    
    M(1,1) += IC(1,1);
    M(1,2) += IC(1,2);
    M(1,3) += IC(1,1) * z;
    M(1,4) += IC(1,2) * z;
    B(1)   += u * IC(1,1) + v * IC(1,2);
    
    M(2,1) += IC(2,1);
    M(2,2) += IC(2,2);
    M(2,3) += IC(2,1) * z;
    M(2,4) += IC(2,2) * z;
    B(2)   += u * IC(2,1) + v * IC(2,2);
    
    M(3,1) += IC(1,1) * z;
    M(3,2) += IC(1,2) * z;
    M(3,3) += IC(1,1) * z * z;
    M(3,4) += IC(1,2) * z * z;
    B(3)   += ( u * IC(1,1) + v * IC(1,2) ) * z;
    
    M(4,1) += IC(2,1) * z;
    M(4,2) += IC(2,2) * z;
    M(4,3) += IC(2,1) * z * z;
    M(4,4) += IC(2,2) * z * z;
    B(4)   += ( u * IC(2,1) + v * IC(2,2) ) * z;
  }
  
  HepVector p = solve(M, B);
  
  // Update member variables 
  // Note that origin has local z = 0

  protoIntercept = LocalPoint(p(1), p(2), 0.);
  protoSlope_u = p(3);
  protoSlope_v = p(4);
}


/* Method fillChiSquared
 *
 * Determine Chi^2 for the proto wire segment
 *
 */
void CSCSegAlgoDF::fillChiSquared() {
  
  double chsq = 0.;
  
  ChamberHitContainer::const_iterator ih;
  for (ih = protoSegment.begin(); ih != protoSegment.end(); ++ih) {
    
    const CSCRecHit2D& hit = (**ih);
    const CSCLayer* layer  = theChamber->layer(hit.cscDetId().layer());
    GlobalPoint gp         = layer->toGlobal(hit.localPosition());
    LocalPoint lp          = theChamber->toLocal(gp);
    
    double u = lp.x();
    double v = lp.y();
    double z = lp.z();
    
    double du = protoIntercept.x() + protoSlope_u * z - u;
    double dv = protoIntercept.y() + protoSlope_v * z - v;
    
    HepMatrix IC(2,2);
    IC(1,1) = hit.localPositionError().xx();
    IC(1,2) = hit.localPositionError().xy();
    IC(2,2) = hit.localPositionError().yy();
    IC(2,1) = IC(1,2);
    
    // Invert covariance matrix
    int ierr = 0;
    IC.invert(ierr);
    if (ierr != 0) {
      LogDebug("CSC") << "CSCSegment::fillChiSquared: failed to invert covariance matrix=\n" << IC << "\n";
      
    }
    
    chsq += du*du*IC(1,1) + 2.*du*dv*IC(1,2) + dv*dv*IC(2,2);
  }

  protoChi2 = chsq;
}


/* fillLocalDirection
 *
 */
void CSCSegAlgoDF::fillLocalDirection() {
  // Always enforce direction of segment to point from IP outwards
  // (Incorrect for particles not coming from IP, of course.)
  
  double dxdz = protoSlope_u;
  double dydz = protoSlope_v;
  double dz   = 1./sqrt(1. + dxdz*dxdz + dydz*dydz);
  double dx   = dz*dxdz;
  double dy   = dz*dydz;
  LocalVector localDir(dx,dy,dz);
  
  // localDir may need sign flip to ensure it points outward from IP
  // ptc: Examine its direction and origin in global z: to point outward
  // the localDir should always have same sign as global z...
  
  double globalZpos    = ( theChamber->toGlobal( protoIntercept ) ).z();
  double globalZdir    = ( theChamber->toGlobal( localDir ) ).z();
  double directionSign = globalZpos * globalZdir;
  protoDirection       = (directionSign * localDir).unit();
}


/* isHitNearSegment
 *
 * Compare rechit with expected position from proto_segment and use 
 * rechit error ellipse x factor (5 or 10 ?) to determine if close enough
 */
bool CSCSegAlgoDF::isHitNearSegment( const CSCRecHit2D* hit) const {

  const CSCLayer* layer  = theChamber->layer(hit->cscDetId().layer());
  GlobalPoint gp         = layer->toGlobal(hit->localPosition());
  LocalPoint lp          = theChamber->toLocal(gp);

  double u = lp.x();
  double v = lp.y();                     
  double z = lp.z();  

  double sigma_u = sqrt( hit->localPositionError().xx() );
  double sigma_v = sqrt( hit->localPositionError().yy() );
  // double sigma_uv = hit->LocalPositionError().xy();

  double du = protoIntercept.x() + protoSlope_u * z - u;
  double dv = protoIntercept.y() + protoSlope_v * z - v;
  
  // For now, ignore correlations so we don't have to play with the 
  // error ellipse, but can add fix in future if seems necessary.

  if (( du < nSigmaFromSegment * sigma_u && du > -nSigmaFromSegment * sigma_u ) &&
      ( dv < nSigmaFromSegment * sigma_v && dv > -nSigmaFromSegment * sigma_v )) 
    return true;

  return false;
}


/* hasHitOnLayer
 *
 * Just make sure hit to be added to layer comes from different layer than those in proto segment   
 */
bool CSCSegAlgoDF::hasHitOnLayer(int layer) const {
  
  // Is there already a hit on this layer?
  for ( ChamberHitContainerCIt it = protoSegment.begin(); it != protoSegment.end(); it++ )
    if ( (*it)->cscDetId().layer() == layer ) return true;
  
  return false;
}


/* Method compareProtoSegment
 *      
 * For hit coming from the same layer of an existing hit within the proto segment
 * test if achieve better chi^2 by using this hit than the other
 *
 */ 
void CSCSegAlgoDF::compareProtoSegment(const CSCRecHit2D* h, int layer) {
  
  // Store old segment first
  double old_protoChi2                  = protoChi2;
  LocalPoint old_protoIntercept         = protoIntercept;
  float old_protoSlope_u                = protoSlope_u;
  float old_protoSlope_v                = protoSlope_v;
  LocalVector old_protoDirection        = protoDirection;
  ChamberHitContainer old_protoSegment  = protoSegment;
 
  bool ok = replaceHit(h, layer);
  
  if ( (protoChi2 > old_protoChi2) || ( !ok ) ) {
    protoChi2       = old_protoChi2;
    protoIntercept  = old_protoIntercept;
    protoSlope_u    = old_protoSlope_u;
    protoSlope_v    = old_protoSlope_v;
    protoDirection  = old_protoDirection;
    protoSegment    = old_protoSegment;
  }
}


/* Method replaceHit
 * 
 * Try adding the hit to existing segment, and remove old one existing in same layer
 *
 */
bool CSCSegAlgoDF::replaceHit(const CSCRecHit2D* h, int layer) {
  
  // replace a hit from a layer
  ChamberHitContainer::iterator it;
  for ( it = protoSegment.begin(); it != protoSegment.end(); ) {
    if ( (*it)->cscDetId().layer() == layer ) {
      it = protoSegment.erase(it);
    } else {
      ++it;
    }
  }
  return addHit(h, layer);
}


/* Method increaseProtoSegment
 *
 * For hit coming from different layer of an existing hit within the proto segment
 * see how far it falls from projected segment position and add if close enough
 *
 */     
void CSCSegAlgoDF::increaseProtoSegment(const CSCRecHit2D* h, int layer) {
  
  // Store old segment first
  double old_protoChi2                  = protoChi2;
  LocalPoint old_protoIntercept         = protoIntercept;
  float old_protoSlope_u                = protoSlope_u;
  float old_protoSlope_v                = protoSlope_v;
  LocalVector old_protoDirection        = protoDirection;
  ChamberHitContainer old_protoSegment  = protoSegment;

  // Test that new hit fits closely to existing segment  
  bool ok = addHit(h, layer);
  
  if ( !ok ) {
    protoChi2       = old_protoChi2;
    protoIntercept  = old_protoIntercept;
    protoSlope_u    = old_protoSlope_u;
    protoSlope_v    = old_protoSlope_v;
    protoDirection  = old_protoDirection;
    protoSegment    = old_protoSegment;
  }
}


/* Method isSegmentGood
 *
 * Look at how many wire hit we have in chamber
 * If the chamber has 20 hits or fewer, require at least 3 hits on segment
 * If the chamber has >20 hits require at least 4 hits
 *
 */
bool CSCSegAlgoDF::isSegmentGood(const ChamberHitContainer& RecHitsInChamber) const {

  unsigned int iadd = ( RecHitsInChamber.size() > 20 )? iadd = 1 : 0;  

  if (protoSegment.size() >= minHitsPerSegment+iadd) return true;

  return false;
}


/* Method flagHitsAsUsed
 *
 * Flag hits which have entered segment building so we don't reuse them.
 */
void CSCSegAlgoDF::flagHitsAsUsed(const ChamberHitContainer& rechitsInChamber) {
  
  // Flag hits on segment as used
  ChamberHitContainerCIt ib = rechitsInChamber.begin();
  ChamberHitContainerCIt hi, iu;
  
  for ( hi = protoSegment.begin(); hi != protoSegment.end(); ++hi ) {
    for ( iu = ib; iu != rechitsInChamber.end(); ++iu ) {
      if (*hi == *iu) usedHits[iu-ib] = true;
    }
  }
}


/* weightMatrix
 *   
 */
AlgebraicSymMatrix CSCSegAlgoDF::weightMatrix() const {
  
  std::vector<const CSCRecHit2D*>::const_iterator it;
  int nhits = protoSegment.size();
  AlgebraicSymMatrix matrix(2*nhits, 0);
  int row = 0;
  
  for (it = protoSegment.begin(); it != protoSegment.end(); ++it) {
    
    const CSCRecHit2D& hit = (**it);
    ++row;
    matrix(row, row)   = hit.localPositionError().xx();
    matrix(row, row+1) = hit.localPositionError().xy();
    ++row;
    matrix(row, row-1) = hit.localPositionError().xy();
    matrix(row, row)   = hit.localPositionError().yy();
  }
  int ierr;
  matrix.invert(ierr);
  return matrix;
}


/* derivativeMatrix
 *
 */
HepMatrix CSCSegAlgoDF::derivativeMatrix() const {
  
  ChamberHitContainer::const_iterator it;
  int nhits = protoSegment.size();
  HepMatrix matrix(2*nhits, 4);
  int row = 0;
  
  for(it = protoSegment.begin(); it != protoSegment.end(); ++it) {
    
    const CSCRecHit2D& hit = (**it);
    const CSCLayer* layer = theChamber->layer(hit.cscDetId().layer());
    GlobalPoint gp = layer->toGlobal(hit.localPosition());      
    LocalPoint lp = theChamber->toLocal(gp); 
    float z = lp.z();
    ++row;
    matrix(row, 1) = 1.;
    matrix(row, 3) = z;
    ++row;
    matrix(row, 2) = 1.;
    matrix(row, 4) = z;
  }
  return matrix;
}


/* calculateError
 *
 */
AlgebraicSymMatrix CSCSegAlgoDF::calculateError() const {
  
  AlgebraicSymMatrix weights = weightMatrix();
  AlgebraicMatrix A = derivativeMatrix();
  
  // (AT W A)^-1
  // from http://www.phys.ufl.edu/~avery/fitting.html, part I
  int ierr;
  AlgebraicSymMatrix result = weights.similarityT(A);
  result.invert(ierr);
  
  // blithely assuming the inverting never fails...
  return result;
}

