#include <algorithm>
#include <vector>
#include <cmath>
#include <numeric>
#include "G4PhysicalConstants.hh"

#include "BreakupReaction.hh"

BreakupReaction::BreakupReaction(G4VPreCompoundModel* ptr):
G4HadronicInteraction("Breakup"),
theProjectileFragmentation(ptr),
pA(0), pZ(0), tA(0), tZ(0), spectatorA(0), spectatorZ(0),
projectile3dNucleus(0), target3dNucleus(0)
{}

BreakupReaction::~BreakupReaction()
{}

void BreakupReaction::ModelDescription(std::ostream& outFile) const
{
    outFile << "Breakup \n";
}

struct ReactionProduct4Momentum
{
    G4LorentzVector operator()(
        G4LorentzVector a,
        G4ReactionProduct* b
    )
    {
        return a + G4LorentzVector(b->GetMomentum(), b->GetTotalEnergy() );
    }
};

G4HadFinalState* BreakupReaction::ApplyYourself(
    const G4HadProjectile& aTrack,
    G4Nucleus& tNucleus 
)
{
	pA=aTrack.GetDefinition()->GetBaryonNumber();
	//pZ=G4lrint(aTrack.GetDefinition()->GetPDGCharge()/eplus);
	tA=tNucleus.GetA_asInt();
	tZ=tNucleus.GetZ_asInt();

    G4double tM=G4ParticleTable::GetParticleTable()->GetIonTable()->GetIonMass(tZ,tA);
	
	G4LorentzVector incMom(aTrack.Get4Momentum());
    G4LorentzVector tarMom(0,0,0,tM);    
    
    // 	G4cout << "proj mom : " << incMom << G4endl;
    // 	G4cout << "target mom : " << tarMom << G4endl;
    //  G4double iM=G4ParticleTable::GetParticleTable()->GetIonTable()->GetIonMass(pZ,pA);	
    // 	G4cout << "Fusion: "  <<  incMom.theta()*180/pi << " " << (incMom.e()-iM) << G4endl;
	
	// transform into CM frame 
	const G4ThreeVector    cmBoost = incMom.findBoostToCM(tarMom);//The boost from CM to lab
    const G4LorentzVector cmincMom = incMom.boost(cmBoost);
	const G4LorentzVector cmtarMom = tarMom.boost(cmBoost);
	
//   G4cout << "proj mom : " << cmincMom << G4endl;
// 	G4cout << "target mom : " << cmtarMom << G4endl;
	
	
    G4double eInc = incMom.e() + tarMom.e(); // relative total energy
	
	G4ParticleDefinition*  products[2];
	GetBreakupProducts( pA, pZ, tA, tZ, products);
		
	G4double rM=products[0]->GetPDGMass(); 
    G4double sM=products[1]->GetPDGMass();
	
//  	G4cout << "recoil: " << rM << " scatter: " << sM << G4endl;
//  	G4cout << "incEnergy : " << eInc << " resEnergy: " << eRes << " Q: " << (eRes-eInc) << G4endl;
	
	// divide energy
	/* Momentum conservation: pc^2 = Er^2-Mr^2 = Es^2-Ms^2  with M = m0*c^2
	 * Energy conservation  : E = Er + Es = Mr + Ms + Q
	 * 
	 *				E^2 - Ms^2 + Mr^2
	 * ->     Er =  -----------------
	 *   			      2E
	 */
	
	// energy
    G4double rE = (eInc*eInc - sM*sM + rM*rM)/(2*eInc);                        //recoil energy d
    G4double sE = eInc - rE;		                                           //scatter energy n
	
	// momentum
	G4double rP = sqrt(rE*rE-rM*rM);
	G4double sP = rP;                 // Momentum conservation or sqrt(sE*sE-sM*sM);
	
	// Lorentz vectors in CM Frame
	G4LorentzVector cmrMom = G4LorentzVector(0.0,0.0,-rP,rE);
	G4LorentzVector cmsMom = G4LorentzVector(0.0,0.0, sP,sE);
// 	G4cout << "recoil  mom (CM): " << cmrMom << G4endl;
// 	G4cout << "scatter mom (CM): " << cmsMom << G4endl;
	
    //MODIFY THIS FOR ANGULAR CROSS SECTION
	//vertex position uniform in spherical shell
    G4ThreeVector normalVector=G4ThreeVector(1.,1.,1.);
    G4ThreeVector ur;                                       //CM frame
    G4double prob;
    G4double angle;

    do {
        // Angular dependance of the cross section
        angle = G4UniformRand()*twopi;
        prob = Probability_Calculator(angle);
        /*
        if(0.88*MeV<=eInc && eInc<1.32*MeV){prob=Prob1(angle);}                //1.10MeV              //sE CORRECT ENERGY???
        else if(1.32*MeV<=eInc && eInc<1.75*MeV){prob=Prob2(angle);}           //1.54MeV
        else if(1.75*MeV<=eInc && eInc<2.15*MeV){prob=Prob3(angle);}           //1.95MeV
        else if(2.15*MeV<=eInc && eInc<2.55*MeV){prob=Prob4(angle);}           //2.35MeV
        else if(2.55*MeV<=eInc && eInc<3.00*MeV){prob=Prob5(angle);}           //2.82MeV
        else if(3.00*MeV<=eInc && eInc<3.35*MeV){prob=Prob6(angle);}           //3.14MeV
        else {prob=1*(angle<6.2832);}
        */
    } while(G4UniformRand()>=prob);
    G4double cosTheta = std::cos(angle);
    G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    G4double phi      = twopi*G4UniformRand();  //phi uniform in [0, 2*pi]
    ur=G4ThreeVector(sinTheta*std::cos(phi),sinTheta*std::sin(phi),cosTheta);

    G4cout << normalVector.angle(ur) << G4endl;

	cmrMom = G4LorentzVector(ur*rP,rE);
	cmsMom = G4LorentzVector(-ur*sP,sE);
	
// 	G4cout << "recoil  mom : " << cmrMom << G4endl;
// 	G4cout << "scatter mom : " << cmsMom << G4endl;
	
	//transform into Lab frame
	G4LorentzVector labrMom = cmrMom.boost(-cmBoost);
	G4LorentzVector labsMom = cmsMom.boost(-cmBoost);
	 
//  	G4cout << "recoil  mom (Lab) : " << labrMom << G4endl;
//  	G4cout << "scatter mom (Lab) : " << labsMom << G4endl;
	
//      G4cout << "Fusion: "  <<  labsMom.theta()*180/pi << " " << (labsMom.t()-sM) << G4endl;
	
	// store particle info
	theResult.Clear();
    theResult.SetStatusChange(stopAndKill);  //isAlive alternative;
	
    G4DynamicParticle * rPart = new G4DynamicParticle(products[0], labrMom);
//  	rPart->GetDefinition()->GetProcessManager()->StartTracking();
	
	theResult.AddSecondary(rPart);
	
	G4DynamicParticle * sPart = new G4DynamicParticle(products[1], labsMom);
//  	sPart->GetDefinition()->GetProcessManager()->StartTracking();	
	theResult.AddSecondary(sPart);

	return &theResult;
}

void BreakupReaction::GetBreakupProducts(
    G4double m_pA, G4double m_pZ,
    G4double m_tA, G4double m_tZ,
    G4ParticleDefinition* products[]
)
{
    G4cout << "HERE!\n";
    // the pA, pZ, tA, tZ refer to the protons and nucleons of reactants. 
    // Naming convention comes from Christian. Not going to refractor it for now. 
    // If the reactants are a neutron and deuteron, set the breakup products.
    if ( (pA == 2 && pZ == 1 && tA == 0 && tZ == 1) || (pA == 0 && pZ == 1 && tA == 2 && tZ == 1) )
    {
        products[0] = G4Neutron::Neutron();
        products[1] = G4Neutron::Neutron();
        products[2] = G4Proton::Proton();
    }

    /* *** Need to change this to p,n,n (maybe gammas)
    G4ParticleDefinition* Boron10 = G4IonTable::GetIonTable()->GetIon(5,10);


    if ( (pA == 2 && pZ == 1 && tA == 9 && tZ == 4) || (pA == 9 && pZ == 4 && tA == 2 && tZ == 1) ) {
        products[0] = Boron10;
        products[1] = G4Neutron::Neutron();

        G4cout << "Boron10 generated" << G4endl;

    }

    G4cout << "GetFusionProducts called."<< G4endl;
    */
    return;
}

//-----------------------------------------------------------------------------------
//functions for angular cross section

G4double BreakupReaction::Probability_Calculator(const G4double angle)
{
    const int N = 0;

    G4double angles[N] = {};
    G4double data[N] = {};

    G4PhysicsOrderedFreeVector interpVect(angles, data, N);
    double cross = interpVect.Value(angle/pi*180);

    return cross;
}

