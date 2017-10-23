#include <algorithm>
#include <vector>
#include <cmath>
#include <numeric>

#include "BreakupReaction.hh"

BreakupReaction::BreakupReaction(G4VPreCompoundModel* ptr):
G4HadronicInteraction("Breakup"),
theProjectileFragmentation(ptr),
pA(0),pZ(0), tA(0),tZ(0),spectatorA(0),spectatorZ(0),
projectile3dNucleus(0),target3dNucleus(0)
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
	const G4ThreeVector   cmBoost = incMom.findBoostToCM(tarMom);//The boost from CM to lab
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
	
	// devide energy
	/* Momentum conservation: pc^2 = Er^2-Mr^2 = Es^2-Ms^2  with M = m0*c^2
	 * Energy conservation  : E = Er + Es = Mr + Ms + Q
	 * 
	 *				E^2 - Ms^2 + Mr^2
	 * ->     Er =  -----------------
	 *   			      2E
	 */
	
	// energy
    G4double rE = (eInc*eInc - sM*sM + rM*rM)/(2*eInc);                        //recoil energy B10
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

        angle=G4UniformRand()*6.2832;
        prob=Prob6(angle);
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
    G4double phi      = 6.2832*G4UniformRand();  //phi uniform in [0, 2*pi]
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

G4double BreakupReaction::Prob1(const G4double angle)
{
    //P1 deuteron energy 1.10

    const int N=20;

    G4double angles[N] = {0.796883615859382, 4.41416602927832, 10.5530158843095, 15.7982176844127, 21.5870079510485, 26.4366139561357, 31.7045838595492, 37.0294740212383, 41.913232181291, 53.6587274764759, 62.9395755883242, 72.3911844749995, 82.9612764367918, 103.108201853465, 112.53704263683, 122.734306906917, 131.383340151906, 140.709724470374, 151.043597360323, 159.228730500364};
    G4double data[N] = {7.63503441896867E-02, 7.18633913801384E-02, 7.49268396805351E-02, 0.070384389619168, 7.89143172237146E-02, 0.075476262473541, 7.31148544086519E-02, 7.62060513349581E-02, 7.60395595795018E-02, 7.01845995126203E-02, 6.22318433269893E-02, 7.06369021149434E-02, 6.91856489798824E-02, 8.81351856134049E-02, 0.094359202404881, 9.61933865774917E-02, 0.105716714989594, 0.102126042796919, 0.117046478948398, 0.121131076682261};

    G4PhysicsOrderedFreeVector interpVect(angles, data, N);
    double cross = interpVect.Value(angle/3.1416*180);

    return cross;
}

G4double BreakupReaction::Prob2(const G4double angle)
{
    //P2 deuteron energy 1.54

    const int N=18;

    G4double angles[N] = {1.2495551425521, 6.00419154573132, 15.8772588872632, 21.685317727075, 32.7035469967179, 43.7059591126576, 53.7814860215903, 63.9202815453359, 74.0116256079718, 84.1662382854205, 99.078650796789, 104.462809917355, 113.943611847048, 123.72177626636, 132.731227015698, 142.022223100952, 150.639408438451, 159.977855984815};
    G4double data[N] = {9.79402902447703E-02, 8.69846969037921E-02, 9.28819644904899E-02, 0.100049982205702, 9.98834275772074E-02, 9.85078097196409E-02, 8.86810866384594E-02, 8.36906164735655E-02, 7.50729566214559E-02, 7.12915496856339E-02, 8.79963620546482E-02, 9.39614852307327E-02, 0.101073984736447, 9.97168729487129E-02, 0.101999288228083, 9.46030289849341E-02, 9.81006761833208E-02, 9.43316066273873E-02};

    G4PhysicsOrderedFreeVector interpVect(angles, data, N);
    double cross = interpVect.Value(angle/3.1416*180);

    return cross;
}

G4double BreakupReaction::Prob3(const G4double angle)
{
    //P3 deuteron energy 1.95

    const int N=18;

    G4double angles[N] = {-0.234977358952388, 3.60298617060334, 10.6327254925957, 15.567250030596, 22.3815934402153, 32.6226900012238, 43.54913719251, 53.6335821808836, 63.2480724513523, 73.82205360421, 83.612776893893, 104.055807122751, 113.670297393219, 123.167298984212, 132.664300575204, 143.140374495165, 152.108677028515, 159.412556602619};
    G4double data[N] = {9.03842858891199E-02, 9.89915555011626E-02, 0.101358952392608, 0.127282584750948, 0.142034267531513, 0.139399583894259, 0.128087626973443, 0.115550605801003, 9.93065720230081E-02, 9.17143556480234E-02, 8.66104515971115E-02, 0.104865499938808, 0.114621466160813, 0.116950679231428, 0.119279892302044, 0.105498714967568, 0.100407538856933, 9.41040264349527E-02};

    G4PhysicsOrderedFreeVector interpVect(angles, data, N);
    double cross = interpVect.Value(angle/3.1416*180);

    return cross;
}

G4double BreakupReaction::Prob4(const G4double angle)
{
    //P4 deuteron energy 2.35

    const int N=16;

    G4double angles[N] = {1.13202126131301, 10.2571469616434, 21.1981037207297, 31.4437580807355, 43.2409136618301, 53.3141789972704, 63.795431690849, 74.7306421491164, 83.7408418330699, 103.847148398218, 113.673322798448, 122.206579514437, 132.791265622755, 141.698031891969, 150.909352104582, 159.362160609107};
    G4double data[N] = {7.13944835512138E-02, 8.53002442177847E-02, 0.116708662548484, 0.120680936647033, 0.116899152420629, 0.104399655221951, 9.18833500933773E-02, 0.083742709380836, 8.66672891825886E-02, 9.68248814825456E-02, 9.97158454245079E-02, 9.60685246372647E-02, 9.34352822870277E-02, 8.64767993104439E-02, 6.96184456256285E-02, 5.82842982330124E-02};

    G4PhysicsOrderedFreeVector interpVect(angles, data, N);
    double cross = interpVect.Value(angle/3.1416*180);

    return cross;
}

G4double BreakupReaction::Prob5(const G4double angle)
{
    //P5 deuteron energy 2.82

    const int N=17;

    G4double angles[N] = {-0.21761105699424, 10.1012700104761, 20.8936021614071, 25.0987888033229, 31.5683067139627, 41.85484019188, 51.3709129004392, 62.0015071036042, 71.6704956900512, 82.962745134077, 101.944898821886, 111.211012883897, 120.512415225422, 131.722325350585, 137.327280413166, 148.940065062765, 159.403039938245};
    G4double data[N] = {5.58037824624602E-02, 7.93490047602418E-02, 0.109709496590638, 0.126640151445533, 0.128687312760756, 0.110102299251962, 9.49607601683545E-02, 7.06700729658695E-02, 7.03260122406218E-02, 0.071062875627194, 7.49421603043615E-02, 7.46124354426658E-02, 7.76975132790531E-02, 7.04665037034314E-02, 6.68509989156205E-02, 5.96056534764469E-02, 5.80946534580676E-02};

    G4PhysicsOrderedFreeVector interpVect(angles, data, N);
    double cross = interpVect.Value(angle/3.1416*180);

    return cross;
}

G4double BreakupReaction::Prob6(const G4double angle)
{
    //P6 deuteron energy 3.14

    const int N=16;

    G4double angles[N] = {-0.772379053415884, 9.90660090250811, 20.7031168013432, 32.4818973659355, 42.0778675621786, 52.2531220484835, 62.6130758736488, 72.6120264455871, 83.0643299401826, 102.113548116276, 112.179662084164, 121.884772798824, 130.674782243677, 140.295938713401, 148.338755378318, 157.187532794626};
    G4double data[N] = {5.74324693042291E-02, 8.32878581173259E-02, 0.124308321964529, 0.13606548431105, 0.12618553888131, 8.70477489768076E-02, 7.17407912687584E-02, 6.18553888130968E-02, 5.84638472032742E-02, 7.22892223738062E-02, 7.10695770804911E-02, 7.52714870395634E-02, 6.54024556616643E-02, 5.87721691678035E-02, 5.64965893587993E-02, 5.42100954979536E-02};

    G4PhysicsOrderedFreeVector interpVect(angles, data, N);
    double cross = interpVect.Value(angle/3.1416*180);

    return cross;
}