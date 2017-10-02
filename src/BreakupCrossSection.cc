#include <algorithm>
#include <vector>
#include <cmath>
#include <numeric>

#include "BreakupCrossSection.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4DynamicParticle.hh"
#include "G4NistManager.hh"
#include "G4ParticleTable.hh"
#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"

#include "G4PhysicsVector.hh"

// ENTER ENERGY DEPENDENT CROSS SECTION HERE (DESCRETE POINTS)

// ENTER Z'S FOR WHICH THE CROSS SECTION APPLIES
static const G4int ZTargetApply = 1;    // hydrogen -> deuterium

// ENTER MAX/MIN ENERGY
BreakupCrossSection::BreakupCrossSection():
G4VCrossSectionDataSet("Breakup")
/*upperLimit( 2*MeV ), lowerLimit( 0*MeV ), r0 ( 1.1 )