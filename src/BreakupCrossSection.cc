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

// ENTER ENERGY DEPENDENT CROSS SECTION HERE (DISCRETE POINTS)
// {deuteron energy keV, cross section mbarn/sr

// ENTER Z's FOR WHICH CROSS SECTION APPLIES
static const G4int ZTargetApply=1;      //d
static const G4int ZProjectileApply=0;  //n    

G4double tM = 1.8756*GeV;

// Enter Max/Min Energy
BreakupCrossSection::BreakupCrossSection():
G4VCrossSectionDataSet(/****Not sure what goes here ***/),
upperLimit( 16*MeV ),
lowerLimit(  0*MeV ),
r0( 1.1 /*Check this usage*/)
{}

BreakupCrossSection::~BreakupCrossSection()
{}

void BreakupCrossSection:: CrossSectionDescription(std::ostream& outFile) const
{
    outFile << "BreakupCrossSection calculates the total reaction spectra\n"
            << "for arbitrary input cross section\n";
}

G4bool BreakupCrossSection::IsElementApplicable(
    const G4DynamicParticle* aDParticle,
    G4int Z, 
    const G4Material* aMaterial
)// Z is the target Z
{
    G4int tA = 0;
    G4int tZ = 0;

    // Projectile Baryon and Atomic Numbers
    G4int pA = aDParticle->GetDefinition()->GetBaryonNumber();
    G4int pZ = aDParticle->GetDefinition()->GetAtomicNumber();

    const G4ElementVector* elements = aMaterial->GetElementVector();
    for(unsigned int i=0; i < elements -> size(); i++)
    {
        tA = (*elements)[i]->GetIsotope(0)->GetN();
        tZ = Z;
    }

    return (pZ == ZProjectileApply);
}

G4double BreakupCrossSection::GetElementCrossSection(
    const G4DynamicParticle* aDParticle,
    G4int tZ,
    G4double tM,
    const G4Material* aMaterial
)
{
    // A == target A
    G4int tA = aMaterial->GetElement(0)->GetIsotope(0)->GetN();

    // Projectile Baryon and Atomic Numbers
    G4int pA = aDParticle->GetDefinition()->GetBaryonNumber();
    G4int pZ = aDParticle->GetDefinition()->GetAtomicNumber();

    if (tZ != ZTargetApply) return 0.0;

    G4double xsection = GetIsoCrossSection( aDParticle, tZ, tA, tM );

    G4double nDens = aMaterial->GetElement(0)->GetA();

    return xsection;
}

G4double BreakupCrossSection::GetIsoCrossSection(
    const G4DynamicParticle* aDParticle,
    G4int tZ,
    G4int tA,
    G4double tM,
    const G4Isotope*,
    const G4Element*,
    const G4Material*
)
{
    if(tZ!=ZTargetApply) return 0.0;
    // calculate CM energy in keV
    G4double E = GetCMEnergy(aDParticle,tZ,tA,tM)*1000.0;

    G4int pZ = aDParticle->GetDefinition()->GetAtomicNumber();
    G4int pA = aDParticle->GetDefinition()->GetBaryonNumber();

    G4double xsection = CalcCrossSection(E)*millibarn*4*CLHEP::pi; // xsection is in units of mbarn/sr
    
    return xsection;
}

G4double BreakupCrossSection::GetCMEnergy(
    const G4DynamicParticle* aDParticle,
    G4int tZ,
    G4int tA,
    G4double tM
)
{
    // projectile
    const G4double pM = aDParticle->GetMass();
    G4LorentzVector incMom = aDParticle->Get4Momentum();

    // target
    // const G4double tM = G4ParticleTable::GetParticleTable()->FindIon(tZ,tA,0,0)->GetPDGMass();
    //const G4double tM = 4.5*GeV;                  //MANUALLY ENTERED TARGET MASS
    G4LorentzVector tarMom(0,0,0,tM);
    
    // 	G4cout << "proj mom : " << incMom << "target mom : " << tarMom << " pM " << pM << " tM " << tM << G4endl;

	// transform into CM frame 
	const G4ThreeVector   cmBoost = incMom.findBoostToCM(tarMom);//The boost from CM to lab
	const G4LorentzVector cmincMom = incMom.boost(cmBoost);
	const G4LorentzVector cmtarMom = tarMom.boost(cmBoost);

	G4double eCM = (incMom.e() + tarMom.e())- (pM + tM); // Kinetic Energy = Total energy - Rest Energy*/
  
//     G4cout << "proj mom : " << cmincMom << "target mom : " << cmtarMom << " " << " KE " << eCM << G4endl;

    return eCM;  // MEV
}

G4double BreakupCrossSection::CalcCrossSection( const G4double E )
{
    // interpolate from discrete datapoints xSec

    const int N = 118; // this is not the right cross section yet
    G4double energies[N]={0.1*MeV, 0.2*MeV, 0.3*MeV, 0.4*MeV, 0.5*MeV, 0.6*MeV, 0.7*MeV, 0.8*MeV, 0.9*MeV, 1.0*MeV, 1.1*MeV, 1.2*MeV, 1.3*MeV, 1.4*MeV, 1.5*MeV, 1.6*MeV, 1.7*MeV, 1.8*MeV, 1.9*MeV, 2.0*MeV, 2.1*MeV, 2.2*MeV, 2.3*MeV, 2.4*MeV, 2.5*MeV, 2.6*MeV, 2.7*MeV, 2.8*MeV, 2.9*MeV, 3.0*MeV, 3.1*MeV, 3.2*MeV, 3.3*MeV, 3.4*MeV, 3.5*MeV, 3.6*MeV, 3.7*MeV, 3.8*MeV, 3.9*MeV, 4.0*MeV, 4.1*MeV, 4.2*MeV, 4.3*MeV, 4.4*MeV, 4.5*MeV, 4.6*MeV, 4.7*MeV, 4.8*MeV, 4.9*MeV, 5.0*MeV, 5.1*MeV, 5.2*MeV, 5.3*MeV, 5.4*MeV, 5.5*MeV, 5.6*MeV, 5.7*MeV, 5.8*MeV, 5.9*MeV, 6.0*MeV, 6.1*MeV, 6.2*MeV, 6.3*MeV, 6.4*MeV, 6.5*MeV, 6.6*MeV, 6.7*MeV, 6.8*MeV, 6.9*MeV, 7.0*MeV, 7.1*MeV, 7.2*MeV, 7.3*MeV, 7.4*MeV, 7.5*MeV, 7.6*MeV, 7.7*MeV, 7.8*MeV, 7.9*MeV, 8.0*MeV, 8.1*MeV, 8.2*MeV, 8.3*MeV, 8.4*MeV, 8.5*MeV, 8.6*MeV, 8.7*MeV, 8.8*MeV, 8.9*MeV, 9.0*MeV, 9.1*MeV, 9.2*MeV, 9.3*MeV, 9.4*MeV, 9.5*MeV, 9.6*MeV, 9.7*MeV, 9.8*MeV, 9.9*MeV, 10.0*MeV, 10.1*MeV, 10.2*MeV, 10.3*MeV, 10.4*MeV, 10.5*MeV, 10.6*MeV, 10.7*MeV, 10.8*MeV, 10.9*MeV, 11.0*MeV, 11.1*MeV, 11.2*MeV, 11.3*MeV, 11.4*MeV, 11.5*MeV, 11.6*MeV, 11.7*MeV, 11.8*MeV};
    G4double data[N]={619.1197713, 711.584257445, 723.321696752, 705.284740515, 675.336672202, 641.854191446, 608.592585698, 577.157278851, 502.400218154, 425.392379139, 376.545686703, 340.638960836, 313.672632191, 293.453091707, 277.629615891, 265.757427199, 256.403964112, 248.992253589, 243.794969359, 239.701710634, 236.785814111, 234.782969786, 233.268640799, 232.347339699, 231.935054388, 231.642456191, 231.41916599, 231.608352243, 231.604638379, 231.670567502, 231.79035795, 231.65134105, 231.603292266, 231.692071429, 231.260142809, 230.724237468, 230.227630596, 229.472313755, 228.694442281, 227.591716065, 226.464525837, 225.190176271, 223.68217161, 221.98283279, 220.187118426, 218.247598338, 215.959092075, 213.546091695, 210.938710429, 208.121789474, 204.987931173, 201.768833655, 198.119494509, 194.142265377, 190.234310608, 186.228156594, 182.094884927, 177.89529497, 173.460580123, 168.887976699, 164.574158417, 160.278048811, 156.073452323, 151.763090734, 147.425608197, 143.366496777, 139.332930861, 135.385067936, 131.185598781, 127.266083286, 123.512377528, 119.940615658, 116.147065268, 112.443626185, 108.933129679, 105.463652287, 102.059108246, 98.5630631928, 95.2844034973, 92.0627395188, 88.8936756054, 85.5135393502, 82.4330398129, 79.4767299427, 76.559522435, 73.5439469905, 70.5828062617, 67.7686363521, 65.0398011205, 62.3652027956, 59.6007902749, 56.8290585171, 54.2650281512, 51.8158034673, 49.4265009428, 47.0413795756, 44.722418236, 42.3593582944, 40.0515476698, 37.943434509, 35.8629084774, 33.8240382153, 31.8630211079, 29.9935249955, 28.2033972872, 26.4728816862, 24.7127315514, 23.222464117, 21.6437808346, 19.8644576859, 18.2602452634, 16.7698620347, 14.7869525777, 13.0015892921, 10.8618341655, 8.15999198364, 4.87413900863, 0.787861413569};

    G4PhysicsOrderedFreeVector interpVect(energies, data, N);
    double cross = interpVect.Value(E);

    //G4cout << "CalcCrossSection called with energy (eV) " << E/eV << " Cross section " << cross << G4endl;


  /*
   G4int i=0;
   while(CrSec[i][0] > E){i++;}

   G4double slopem = (CrSec[i][1]-CrSec[i-1][1])/(CrSec[i][0]-CrSec[i-1][0]);
   G4double intercb = CrSec[i][1]-slopem*CrSec[i][0];

   G4double cross=slopem*E+intercb;

   */


   return cross;
}