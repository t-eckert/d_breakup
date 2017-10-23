#include "TE_HadronInelasticPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "G4GenericIon.hh"
#include "G4IonConstructor.hh"

#include "G4HadronInelasticProcess.hh"
//#include "G4HadronFusionProcess.hh"
#include "G4BinaryLightIonReaction.hh"
#include "BreakupReaction.hh"
// #include "G4TripathiCrossSection.hh"
// #include "G4TripathiLightCrossSection.hh"
#include "G4IonsShenCrossSection.hh"
//#include "G4FusionCrossSection.hh"
//#include "S4IonProtonCrossSection.hh"
//#include "G4GGNuclNuclCrossSection.hh"              //MANUALLY DISABLED

//#include "G4LElastic.hh"

#include "G4PreCompoundModel.hh"
#include "G4ExcitationHandler.hh"
#include "G4FTFBuilder.hh"
#include "G4HadronicInteraction.hh"
#include "G4BuilderType.hh"

#include "G4CrossSectionDataSetRegistry.hh"

using namespace std;

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(TE_HadronInelasticPhysics);

TE_HadronInelasticPhysics::TE_HadronInelasticPhysics(G4int verbosity):
G4VPhysicsConstructor(/* Not sure what goes here *****/),
verbose(verbosity),
wasActivated(false)
{
    /*
    //  fTripathi = fTripathiLight = fShen = fIonH = 0;
    fGGNuclNucl=0;
  theIonBC = 0;
  theFTFP = 0;
  theBuilder = 0;
  SetPhysicsType(bIons);
  if(verbose > 1) { G4cout << "### S4IonPhysics" << G4endl; }
    */
}

TE_HadronInelasticPhysics::TE_HadronInelasticPhysics(const G4String& nname):
G4VPhysicsConstructor(nname),
verbose(1),
wasActivated(false)
{
    /*
    fTripathi = fTripathiLight = fShen = fIonH = 0;
    fGGNuclNucl=0;
  theIonBC = 0;
  theFTFP = 0;
  theBuilder = 0;
  SetPhysicsType(bIons);
  if(verbose > 1) { G4cout << "### S4IonPhysics" << G4endl; }

    */
}

TE_HadronInelasticPhysics::~TE_HadronInelasticPhysics()
{
    delete theBuilder;
}

void TE_HadronInelasticPhysics::ConstructParticle()
{
    // Construct ions
  G4IonConstructor pConstructor;
  pConstructor.ConstructParticle();    
}

void TE_HadronInelasticPhysics::ConstructProcess()
{
    /*
      if(wasActivated) { return; }
  wasActivated = true;

  G4double emax = 100.*TeV;

  G4ExcitationHandler* handler = new G4ExcitationHandler();
  G4PreCompoundModel* thePreCompound = new G4PreCompoundModel(handler);

  // Binary Cascade
//   theIonBC = new G4BinaryLightIonReaction(thePreCompound);
//   theIonBC->SetMinEnergy(0.0);
//   theIonBC->SetMaxEnergy(4*GeV);
  
  theFusion = new G4FusionReaction(thePreCompound);
  theFusion->SetMinEnergy(0.0);
  theFusion->SetMaxEnergy(20*MeV);

  // FTFP
//   theBuilder = new G4FTFBuilder("FTFP",thePreCompound);
//   theFTFP = theBuilder->GetModel();
//   theFTFP->SetMinEnergy(2*GeV);
//   theFTFP->SetMaxEnergy(emax);

//   fShen = new G4IonsShenCrossSection();
  //fTripathi = new G4TripathiCrossSection();
  //fTripathiLight = new G4TripathiLightCrossSection();
  //fIonH = new S4IonProtonCrossSection();
  
   fFusion = new G4FusionCrossSection();
   fFusion->CrossSectionDescription(G4cout);
   
   //const char * default_Name = G4GGNuclNuclCrossSection::Default_Name();                //MANUALLY DISABLED
   //G4cout << "S4IonPhysics::defaultName " << default_Name   << G4endl;
   
//   fGGNuclNucl = G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(default_Name);
//   fGGNuclNucl->SetVerboseLevel(1);
//   fGGNuclNucl->DumpPhysicsTable(*G4Deuteron::Deuteron());
//     
//   AddProcess("dInelastic", G4Deuteron::Deuteron(),false);
//   AddProcess("tInelastic",G4Triton::Triton(),false);
//   AddProcess("He3Inelastic",G4He3::He3(),true);
//   AddProcess("alphaInelastic", G4Alpha::Alpha(),true);
//   AddProcess("ionInelastic",G4GenericIon::GenericIon(),true);
  
   G4HadronFusionProcess* dfusion = new G4HadronFusionProcess("dFusion", G4Deuteron::Deuteron());
   G4ProcessManager* dManager = G4Deuteron::Deuteron()->GetProcessManager();
   dManager->AddDiscreteProcess(dfusion);

 //dfusion->AddDataSet(fGGNuclNucl);
   dfusion->AddDataSet(fFusion);
   dfusion->RegisterMe(theFusion);
   dfusion->DumpInfo();
   /*
   G4HadronFusionProcess* tfusion = new G4HadronFusionProcess("tFusion", G4Triton::Triton());
   G4ProcessManager* tManager = G4Triton::Triton()->GetProcessManager();
   tManager->AddDiscreteProcess(tfusion);
   
   tfusion->AddDataSet(fFusion);
   tfusion->RegisterMe(theFusion);
   tfusion->DumpInfo();
   
   //tManager->DumpInfo();
   

  if(verbose > 1) {
    G4cout << "S4IonPhysics::ConstructProcess done! " 
	   << G4endl;
  }
    */
}

void TE_HadronInelasticPhysics::AddProcess(
    const G4String& name,
    G4ParticleDefinition* particle,
    G4bool
)
{
    /*
  G4HadronInelasticProcess* hadi = new G4HadronInelasticProcess(name, part);
  G4ProcessManager* pManager = part->GetProcessManager();
  pManager->AddDiscreteProcess(hadi);
/*
  hadi->AddDataSet(fShen);
  //hadi->AddDataSet(fTripathi);
  //hadi->AddDataSet(fTripathiLight);
  if(isIon) { hadi->AddDataSet(fIonH); }
 
    
  hadi->AddDataSet(fGGNuclNucl);
    
  hadi->RegisterMe(theIonBC);
  
  hadi->DumpInfo();
  hadi->DumpPhysicsTable(*G4Deuteron::Deuteron());
  hadi->SetVerboseLevel(1);
  
//   hadi->RegisterMe(theFTFP);
    */
}