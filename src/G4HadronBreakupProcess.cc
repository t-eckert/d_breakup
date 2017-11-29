// Breakup Process Class
 
#include "G4HadronBreakupProcess.hh"
#include "G4GenericIon.hh"
#include "G4ParticleDefinition.hh"
  
G4HadronBreakupProcess::G4HadronBreakupProcess(
    const G4String& processName,
    G4ParticleDefinition* aParticle
): G4HadronicProcess(processName, fHadronInelastic) // was fHadronInelastic from G4HadronicProcessType.hh
{
    // AddDataSet(new G4HadronFusionDataSet());
    theParticle = aParticle;
}

G4HadronBreakupProcess::~G4HadronBreakupProcess() 
{}

G4bool G4HadronBreakupProcess::IsApplicable(const G4ParticleDefinition& aP)
{
  G4cout << "G4HadronBreakupProcess::IsApplicable called! "  << G4endl;
  return  theParticle == &aP || theParticle == G4GenericIon::GenericIon();
}